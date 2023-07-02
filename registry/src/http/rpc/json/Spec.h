//
// Created by Ivan Kishchenko on 2019-07-14.
//

#pragma once

#include "optional"
#include "nlohmann/json.hpp"
#include "nlohmann_optional.h"

enum Code {
    ParseError = -32700,    // Parse error	Invalid JSON was received by the server. An error occurred on the server while parsing the JSON text.
    InvalideRequest = -32600,    // Invalid Request	The JSON sent is not a valid Request object.
    MethodNotFound = -32601,    // Method not found	The method does not exist / is not available.
    InvalidParams = -32602,    // Invalid params	Invalid method parameter(s).
    InternalError = -32603,    // Internal error	Internal JSON-RPC error.
    ServerError = -32000, // to -32099	Server error	Reserved for implementation-defined server-errors.
};

/**
 * json-rpc Request
 */
struct JsonRpcRequest {
    std::string method;
    std::optional<nlohmann::json> params;
    std::optional<int> id{};
    std::string jsonrpc = "2.0";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(JsonRpcRequest, method, params, id, jsonrpc)

struct JsonRcpError {
    int code{};
    std::string message;
    std::optional<nlohmann::json> data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(JsonRcpError, code, message, data)

/**
 * json-rpc Response
 */
struct JsonRpcResponse {
    std::optional<nlohmann::json> result;
    std::optional<JsonRcpError> error;
    std::optional<int> id;
    std::string jsonrpc = "2.0";
};

inline void to_json(nlohmann::json &nlohmann_json_j, const JsonRpcResponse &nlohmann_json_t) {
    if (nlohmann_json_t.result) {
        nlohmann_json_j["result"] = nlohmann_json_t.result;
    }
    if (nlohmann_json_t.error) {
        nlohmann_json_j["error"] = nlohmann_json_t.error;
    }
    nlohmann_json_j["id"] = nlohmann_json_t.id;
    nlohmann_json_j["jsonrpc"] = nlohmann_json_t.jsonrpc;
}

inline void from_json(const nlohmann::json &nlohmann_json_j, JsonRpcResponse &nlohmann_json_t) {
    JsonRpcResponse nlohmann_json_default_obj;
    nlohmann_json_t.result = nlohmann_json_j.value("result", nlohmann_json_default_obj.result);
    nlohmann_json_t.error = nlohmann_json_j.value("error", nlohmann_json_default_obj.error);
    nlohmann_json_t.id = nlohmann_json_j.value("id", nlohmann_json_default_obj.id);
    nlohmann_json_t.jsonrpc = nlohmann_json_j.value("jsonrpc", nlohmann_json_default_obj.jsonrpc);
}
