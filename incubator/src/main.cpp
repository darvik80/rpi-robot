#include <tuple>
#include <variant>
#include <functional>
#include <iostream>


struct Item {
    uint16_t id;

};

struct Message {
    uint8_t id;
    std::array<Item, 10> items;
};

int main(int argc, char *argv[]) {
//    Message origin{1, {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}}};
//    char buffer[sizeof (Message)];
//    memcpy(buffer, &origin, sizeof (Message));
//    Message* copy = (Message*)buffer;
//
//
//    std::cout << "is is_trivially_copyable: " << std::is_trivially_copyable<Message>() << std::endl;
//    std::cout << "is trivial: " << std::is_trivial<Message>() << std::endl;
//
//    std::cout << "size: " << sizeof (Message) << std::endl;

    Door door;

    door.handle(OpenEvent{});
    door.handle(CloseEvent{});

    door.handle(CloseEvent{});
    door.handle(OpenEvent{});
    door.handle(OpenEvent{});

    return 0;
}