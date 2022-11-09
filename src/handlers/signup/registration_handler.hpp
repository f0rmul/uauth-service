#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/dynamic_config/storage/component.hpp>

#include <bcrypt.h>
#include "../../repository/repository.hpp"
#include "../../utils/make_error.hpp"

class RegisterHandler final : public userver::server::handlers::HttpHandlerJsonBase
{
public:
  static constexpr std::string_view kName = "sign-up-handler";

  RegisterHandler(const userver::components::ComponentConfig  &config,
                  const userver::components::ComponentContext &context);

  userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest &request,
        const userver::formats::json::Value &request_json,
        userver::server::request::RequestContext &context) const override final;
 
private:
  const UserRepository& repository;
  userver::dynamic_config::Source dynamic_config;
};

 RegisterHandler::RegisterHandler(const userver::components::ComponentConfig  &config,
                                  const userver::components::ComponentContext &context):
                                   userver::server::handlers::HttpHandlerJsonBase(config,context),
                                   repository{context.FindComponent<UserRepository>()},      
                                   dynamic_config{context.FindComponent<userver::components::DynamicConfig>()
                                   .GetSource()}                        
{}

userver::formats::json::Value RegisterHandler::HandleRequestJsonThrow(
                                                 const userver::server::http::HttpRequest &request,
                                                 const userver::formats::json::Value &request_json,
                                                 userver::server::request::RequestContext &context) const 
{
    auto& http_response = request.GetHttpResponse();

    if(request_json.IsEmpty() || 
       !request_json.HasMember("name") ||
       !request_json.HasMember("passwd") ||
       request_json["name"].As<std::string>().empty() ||
       request_json["passwd"].As<std::string>().empty())
       {
            http_response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
            return MakeError("400","Bad credentials");
       }

    auto user_name = request_json["name"].As<std::string>();

    const auto&[rows,user] = repository.GetByName(user_name);
    if(rows != 0)
    {
      http_response.SetStatus(userver::server::http::HttpStatus::kConflict);
      return MakeError("409","User already exists");
    }

    std::string hashed_password = bcrypt::generateHash(request_json["passwd"].As<std::string>());

    std::size_t result = repository.Create(models::User{user_name,hashed_password});

    if(result == 0)
    {
      http_response.SetStatus(userver::server::http::HttpStatus::kInternalServerError);
      return MakeError("500","Unable to create user");
    }
    userver::formats::json::ValueBuilder builder;
    builder["name"] = user_name;
    builder["status"]  = "User was created successfuly";
    return builder.ExtractValue();
}