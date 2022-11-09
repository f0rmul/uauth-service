#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <bcrypt.h>
#include "../tokens/tokens.hpp"
#include "../../repository/repository.hpp"
#include "../../utils/make_error.hpp"


class LogInHandler final : public userver::server::handlers::HttpHandlerJsonBase
{
public:
  static constexpr std::string_view kName = "sign-in-handler";

  LogInHandler(const userver::components::ComponentConfig  &config,
               const userver::components::ComponentContext &context);

  userver::formats::json::Value HandleRequestJsonThrow(
                                  const userver::server::http::HttpRequest &request,
                                  const userver::formats::json::Value &request_json,
                                  userver::server::request::RequestContext &context) const override final;
private:
  const UserRepository& repository;
  const tokens::TokenManager& token_mgr;
};

 LogInHandler::LogInHandler(const userver::components::ComponentConfig  &config,
                                  const userver::components::ComponentContext &context):
                                   userver::server::handlers::HttpHandlerJsonBase(config,context),
                                   repository{context.FindComponent<UserRepository>()},
                                   token_mgr{context.FindComponent<tokens::TokenManager>()}                      
{}

userver::formats::json::Value LogInHandler::HandleRequestJsonThrow(
                                             const userver::server::http::HttpRequest & request,
                                             const userver::formats::json::Value &request_json,
                                             userver::server::request::RequestContext &) const
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
  
  const std::string user_name = request_json["name"].As<std::string>();
  const auto& [rows, user] = repository.GetByName(user_name);

  std::string hashed_password = std::move(user.GetPasswd());
  if(rows == 0 || !bcrypt::validatePassword(request_json["passwd"].As<std::string>(),hashed_password))
  {
    http_response.SetStatusOk();
    return MakeError("200","Invalid username or password");
  }
  std::string access_token = token_mgr.MakeToken(user,tokens::token_type::kAccess);
  std::string refresh_token = token_mgr.MakeToken(user,tokens::token_type::kRefresh);

  std::size_t result = repository.UpdateRefreshToken(user.GetID(),refresh_token);
  if(result == 0)
  {
    http_response.SetStatus(userver::server::http::HttpStatus::kInternalServerError);
    return MakeError("500","Unable to update refresh_token");
  }

  userver::formats::json::ValueBuilder builder;
  builder["access_token"] = access_token;
  builder["refresh_token"] = refresh_token;
  http_response.SetStatusOk();

  return builder.ExtractValue();
}

