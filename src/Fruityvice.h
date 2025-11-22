#ifndef FRUITYVICE_H
#define FRUITYVICE_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Fruityvice {
private:
    std::string api_base = "https://www.fruityvice.com/api";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("www.fruityvice.com"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                // Кодируем значения параметров
                auto encoded_value = web::uri::encode_data_string(utility::conversions::to_string_t(param.second));
                query += param.first + "=" + utility::conversions::to_utf8string(encoded_value);
                first = false;
            }
        }
        return query;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Fruityvice(){
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> get_fruit_by_id(int id) {
        return make_api_call("/fruit/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> get_fruit_by_order(const std::string& order) {
        return make_api_call("/fruit/order/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(order))),"GET");
    }

    pplx::task<json::value> get_fruit_by_family(const std::string& family) {
        return make_api_call("/fruit/family/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(family))),"GET");
    }

    pplx::task<json::value> get_fruit_by_genus(const std::string& genus) {
        return make_api_call("/fruit/genus/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(genus))),"GET");
    }

    pplx::task<json::value> get_fruit_by_name(const std::string& name) {
        return make_api_call("/fruit/" + utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name))),"GET");
    }

    pplx::task<json::value> get_fruits() {
        return make_api_call("/fruit/all","GET");
    }
};

#endif
