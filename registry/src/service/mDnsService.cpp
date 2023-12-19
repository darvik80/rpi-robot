//
// Created by Ivan Kishchenko on 25/10/2023.
//

#include "mDnsService.h"
#include "mdns.h"
#include <iostream>

class ServiceRecord {
public:
    const char *service;
    const char *hostname;
    uint32_t address_ipv4;
    uint8_t *address_ipv6;
    uint16_t port;
};


std::string ipv4AddressToString(char *buffer, size_t capacity, const sockaddr_in *addr, size_t addrlen) {
    char host[NI_MAXHOST] = {0};
    char service[NI_MAXSERV] = {0};
    const int ret = getnameinfo((const struct sockaddr *)addr, (socklen_t)addrlen, host, NI_MAXHOST, service, NI_MAXSERV,
                                NI_NUMERICSERV | NI_NUMERICHOST);
    int len = 0;
    if (ret == 0) {
        if (addr->sin_port != 0) {
            len = snprintf(buffer, capacity, "%s:%s", host, service);
        } else {
            len = snprintf(buffer, capacity, "%s", host);
        }
    }
    if (len >= (int)capacity) {
        len = (int)capacity - 1;
    }

    return std::string(buffer, len);
}

int mdnsRecordCallback(int sock, const struct sockaddr *from, size_t addrlen,
                       mdns_entry_type_t entry, uint16_t query_id, uint16_t rtype,
                       uint16_t rclass, uint32_t ttl, const void *data, size_t size,
                       size_t name_offset, size_t name_length, size_t record_offset,
                       size_t record_length, void *user_data) {
    if (static_cast<int>(entry) != MDNS_ENTRYTYPE_QUESTION) {
        return 0;
    }

    char addrbuffer[64] = {0};
    char namebuffer[256] = {0};
    char sendbuffer[256] = {0};

    std::string fromaddrstr;
    if (from->sa_family == AF_INET) {
        fromaddrstr = ipv4AddressToString(addrbuffer, sizeof(addrbuffer), (const struct sockaddr_in *)from, addrlen);
    }

    if (rtype == static_cast<uint16_t>(mdns_record_type::MDNS_RECORDTYPE_PTR)) {
        const mdns_string_t service =
                mdns_record_parse_ptr(data, size, record_offset, record_length, namebuffer, sizeof(namebuffer));
        std::cout << fromaddrstr << " : question PTR " << std::string(service.str, service.length) << "\n";

        const char dns_sd[] = "_services._dns-sd._udp.local.";
        const ServiceRecord *service_record = (const ServiceRecord *)user_data;
        const size_t service_length = strlen(service_record->service);
        char sendbuffer[256] = {0};

        if ((service.length == (sizeof(dns_sd) - 1)) && (strncmp(service.str, dns_sd, sizeof(dns_sd) - 1) == 0)) {
            std::cout << "  --> answer " << service_record->service << " \n";
//            mdns_query_answer_unicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer), service_record->service,
//                                  service_length);
        } else if ((service.length == service_length) &&
                   (strncmp(service.str, service_record->service, service_length) == 0)) {
            uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
            std::cout << "  --> answer " << service_record->hostname << "." << service_record->service << " port "
                     << service_record->port << " (" << (unicast ? "unicast" : "multicast") << ")\n";
            if (!unicast) addrlen = 0;
            char txt_record[] = "asdf=1";
//            mdns_query_answer_multicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer), query_id, service_record->service,
//                              service_length, service_record->hostname, strlen(service_record->hostname),
//                              service_record->address_ipv4, service_record->address_ipv6, (uint16_t)service_record->port,
//                              txt_record, sizeof(txt_record));
        }
    } else if (rtype == static_cast<uint16_t>(mdns_record_type::MDNS_RECORDTYPE_SRV)) {
        mdns_record_srv_t service =
                mdns_record_parse_srv(data, size, record_offset, record_length, namebuffer, sizeof(namebuffer));
        std::cout << fromaddrstr << " : question SRV  " << std::string(service.name.str, service.name.length) << "\n";
#if 0
        if ((service.length == service_length) &&
		    (strncmp(service.str, service_record->service, service_length) == 0)) {
			uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
			printf("  --> answer %s.%s port %d (%s)\n", service_record->hostname,
			       service_record->service, service_record->port,
			       (unicast ? "unicast" : "multicast"));
			if (!unicast)
				addrlen = 0;
			char txt_record[] = "test=1";
			mdns_query_answer(sock, from, addrlen, sendbuffer, sizeof(sendbuffer), query_id,
			                  service_record->service, service_length, service_record->hostname,
			                  strlen(service_record->hostname), service_record->address_ipv4,
			                  service_record->address_ipv6, (uint16_t)service_record->port,
			                  txt_record, sizeof(txt_record));
		}
#endif
    }
}

void mDnsService::postConstruct(Registry &registry) {
    int sock = mdns_socket_open_ipv4(nullptr);

    while (true) {
        int nfds = 0;
        fd_set readfs{};
        FD_ZERO(&readfs);
        FD_SET(sock, &readfs);

        std::array<uint8_t, 2048> buffer{};

        if (select(nfds, &readfs, nullptr, nullptr, nullptr) >= 0) {
            if (FD_ISSET(sock, &readfs)) {
                mdns_socket_listen(sock, buffer.data(), buffer.size(), mdnsRecordCallback, this);
            }
            FD_SET(sock, &readfs);
        } else {
            break;
        }
    }

    mdns_socket_close(sock);
}

void mDnsService::preDestroy(Registry &registry) {

}
