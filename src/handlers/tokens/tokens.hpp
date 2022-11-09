#pragma once
#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>

#include <jwt/jwt.hpp>
#include "../../models/user.hpp"
#include "../../utils/dynamic_config_parser.hpp"

namespace tokens
{   
    
    struct token_type
    {
        static struct Access {} kAccess;
        static struct Refresh {} kRefresh;
    };
    class TokenManager : public userver::components::LoggableComponentBase
    {   
        public:
        static constexpr std::string_view kName = "token-manager";

        TokenManager(const userver::components::ComponentConfig  &config,
                     const userver::components::ComponentContext &context);


        std::string MakeToken(const models::User&,token_type::Access) const;
        std::string MakeToken(const models::User&,token_type::Refresh)const;

        private:

        template<typename Token>
        std::string ExtractSecret() const;

        int ExtractAccessExpirationTime() const;

        private:
        userver::dynamic_config::Source dynamic_config;
    };

    TokenManager::TokenManager(const userver::components::ComponentConfig  &config,
                               const userver::components::ComponentContext &context) :
                                userver::components::LoggableComponentBase(config,context),
                                dynamic_config{context.FindComponent<userver::components::DynamicConfig>()
                                .GetSource()} {}
    
   
    std::string TokenManager::MakeToken(const models::User& user,token_type::Access)const
    {
        using namespace jwt::params;
        using namespace std::chrono;
        jwt::jwt_object access_token{algorithm("HS256"),
        secret(ExtractSecret<token_type::Access>()),
        payload(
            {
                {"id",std::to_string(user.GetID())},
                {"username",user.GetName()}
            }
        )};
        access_token.add_claim("exp",
        system_clock::now() + seconds{ExtractAccessExpirationTime()});
        return access_token.signature();
    }

    std::string TokenManager::MakeToken(const models::User& user,token_type::Refresh)const
    {
        using namespace jwt::params;
        using namespace std::chrono;
        jwt::jwt_object refresh_token{algorithm("HS256"),
            secret(ExtractSecret<token_type::Refresh>()),
            payload(
            {
                {"id",std::to_string(user.GetID())},
                {"username",user.GetName()}
            }
        )};
        const auto seconds_since_epoch = duration_cast<seconds>(
            system_clock::now().time_since_epoch()
        ).count();
        refresh_token.add_claim("iat",seconds_since_epoch);
        return refresh_token.signature();
    }

    template <typename Token>
    std::string TokenManager::ExtractSecret() const
    {   
        const auto runtime_config = dynamic_config.GetSnapshot();
        if constexpr (std::is_same_v<Token,token_type::Access>)
          return runtime_config[kAccessSecret];
        else if constexpr (std::is_same_v<Token,token_type::Refresh>)
          return runtime_config[kRefreshSecret];
    }

    int TokenManager::ExtractAccessExpirationTime() const
    {
        const auto runtime_config = dynamic_config.GetSnapshot();
        return runtime_config[kAccessExpiration];
    }
}