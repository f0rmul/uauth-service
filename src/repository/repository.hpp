#pragma once

#include <userver/storages/postgres/postgres_fwd.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/dynamic_config/source.hpp>

#include "sql/statements.hpp"
#include "../models/user.hpp"

class UserRepository : public userver::components::LoggableComponentBase
{
    public:
    static constexpr std::string_view kName = "user-repository";
        UserRepository(const userver::components::ComponentConfig&,
                       const userver::components::ComponentContext&);

        std::size_t  Create(const models::User&) const;
        std::pair<std::size_t,models::User> GetByID(std::int32_t) const;
        std::pair<std::size_t,models::User> GetByName(const std::string&) const;
        std::pair<std::size_t,models::User> GetByCredentials(const std::string&,const std::string&) const;
        std::pair<std::size_t,models::User> GetByRefreshToken(const std::string&) const;
        std::size_t UpdateRefreshToken(std::int32_t,const std::string&) const;

    private:
    const userver::storages::postgres::ClusterPtr cluster_;
};

 UserRepository::UserRepository(const userver::components::ComponentConfig& config,
                                const userver::components::ComponentContext& context) : 
                                userver::components::LoggableComponentBase(config,context),
                                cluster_{context.FindComponent<userver::components::Postgres>("postgres-settings").GetCluster()}
{}


std::size_t UserRepository::Create(const models::User& user) const
{
     using userver::storages::postgres::ClusterHostType;
     auto result_set = cluster_->Execute(ClusterHostType::kMaster,
                                         sql::kInsertUser.data(),   
                                         user.GetName(),
                                         user.GetPasswd());

    return result_set.RowsAffected(); 
}   

std::pair<std::size_t,models::User> UserRepository::GetByID(std::int32_t id)const
{
    using userver::storages::postgres::ClusterHostType;
    auto result_set = cluster_->Execute(ClusterHostType::kMaster,
                                        sql::kSelectUserById.data(),
                                        id);
    if(result_set.IsEmpty()) return {};

    auto rows = result_set.AsSetOf<models::User>(userver::storages::postgres::kRowTag);
    static_assert(std::is_same_v<decltype(*rows.begin()), models::User>,"Using non-aggregate class");
    return std::make_pair(result_set.RowsAffected(),*rows.begin());
};

std::pair<std::size_t,models::User> UserRepository::GetByName(const std::string& name)const
{
    using userver::storages::postgres::ClusterHostType;
    auto result_set = cluster_->Execute(ClusterHostType::kMaster,
                                        sql::kSelectUserByName.data(),
                                        name);
    if(result_set.IsEmpty()) return {};

    auto rows = result_set.AsSetOf<models::User>(userver::storages::postgres::kRowTag);
    static_assert(std::is_same_v<decltype(*rows.begin()), models::User>,"Using non-aggregate class");
    return std::make_pair(result_set.RowsAffected(),*rows.begin());
};

std::pair<std::size_t,models::User> UserRepository::GetByCredentials(const std::string& user_name,
                                                                     const std::string& password) const
{
    using userver::storages::postgres::ClusterHostType;
    auto result_set = cluster_->Execute(ClusterHostType::kMaster,
                                        sql::kSelectUserByCredentials.data(),
                                        user_name,
                                        password);
    if(result_set.IsEmpty()) return {};

    auto rows = result_set.AsSetOf<models::User>(userver::storages::postgres::kRowTag);
    static_assert(std::is_same_v<decltype(*rows.begin()), models::User>,"Using non-aggregate class");
    return std::make_pair(result_set.RowsAffected(),*rows.begin());
};

std::pair<std::size_t,models::User> UserRepository::GetByRefreshToken(const std::string& rtoken)const
{
    using userver::storages::postgres::ClusterHostType;
    auto result_set = cluster_->Execute(ClusterHostType::kMaster,
                                        sql::kSelectUserById.data(),
                                        rtoken);
    if(result_set.IsEmpty()) return {};

    auto rows = result_set.AsSetOf<models::User>(userver::storages::postgres::kRowTag);
    static_assert(std::is_same_v<decltype(*rows.begin()), models::User>,"Using non-aggregate class");
    return std::make_pair(result_set.RowsAffected(),*rows.begin());
};

std::size_t UserRepository::UpdateRefreshToken(std::int32_t id,const std::string& rtoken) const
{
    using userver::storages::postgres::ClusterHostType;
     auto result_set = cluster_->Execute(ClusterHostType::kMaster,
                                         sql::kUpdateUsersRToken.data(),   
                                         id,
                                         rtoken);

    return result_set.RowsAffected(); 
};