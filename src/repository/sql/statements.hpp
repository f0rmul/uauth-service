#pragma once
#include <string_view>

namespace sql
{
    inline constexpr std::string_view kInsertUser{
        R"~(
            INSERT INTO users (uname,upasswd)
            VALUES ($1,$2)
        )~"
    };

    inline constexpr std::string_view kSelectUserById{
        R"~(
            SELECT uid,uname,upasswd,urtoken
            FROM users WHERE uid=$1
        )~"
    };

    inline constexpr std::string_view kSelectUserByName{
        R"~(
            SELECT uid,uname,upasswd,urtoken
            FROM users WHERE uname=$1
        )~"
    };
    inline constexpr std::string_view kSelectUserByCredentials{
        R"~(
            SELECT uid,uname,upasswd,urtoken
            FROM users WHERE uname=$1 AND upasswd=$2
        )~"
    };

    inline constexpr std::string_view kSelectUserByRToken{
        R"~(
            SELECT uid,uname,upasswd,urtoken
            FROM users WHERE urtoken=$1
        )~"
    };

    inline constexpr std::string_view kUpdateUsersRToken{
        R"~(
            UPDATE users SET urtoken=$2 WHERE uid=$1
        )~"
    };
} // namespace sql
