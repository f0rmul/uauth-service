#include <userver/components/minimal_server_component_list.hpp>
#include <userver/utils/daemon_run.hpp>

#include <userver/clients/dns/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include "handlers/signin/login_handler.hpp"
#include "handlers/signup/registration_handler.hpp"

int main(int argc, char* argv[])
{
   auto component_list = userver::components::MinimalServerComponentList()
  .Append<LogInHandler>()
  .Append<RegisterHandler>()
  .Append<UserRepository>()
  .Append<tokens::TokenManager>()
  .Append<userver::components::Postgres>("postgres-settings")
  .Append<userver::components::TestsuiteSupport>()
  .Append<userver::clients::dns::Component>();
   return userver::utils::DaemonMain(argc, argv, component_list);
}