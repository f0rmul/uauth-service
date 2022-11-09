#pragma once
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/dynamic_config/value.hpp>
namespace
{
    std::string ParseAccessTokenSecret(const userver::dynamic_config::DocsMap& docs_map)
    { return docs_map.Get("JWT_ACCESS_TOKEN_SECRET").As<std::string>(); }

    std::string ParseRefreshTokenSecret(const userver::dynamic_config::DocsMap& docs_map)
    { return docs_map.Get("JWT_REFRESH_TOKEN_SECRET").As<std::string>();}

    int ParseAccessExpirationTime(const userver::dynamic_config::DocsMap& docs_map)
    { return docs_map.Get("JWT_ACCESS_TOKEN_EXPIRATION").As<int>();}


    constexpr userver::dynamic_config::Key<ParseAccessTokenSecret> kAccessSecret;
    constexpr userver::dynamic_config::Key<ParseRefreshTokenSecret> kRefreshSecret;
    constexpr userver::dynamic_config::Key<ParseAccessExpirationTime> kAccessExpiration;
}