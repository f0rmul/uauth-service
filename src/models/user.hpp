#pragma once
#include <string>
namespace models
{
    class User final
    {
        public:
            User() = default; // required by userver


            User(std::string name,
                 std::string passwd) : 
                            uname{std::move(name)},
                            upasswd{std::move(passwd)},
                            urtoken{"null"}{}
                            
            User(std::string name,
                 std::string passwd,
                 std::string rtoken) : 
                            uname{std::move(name)},
                            upasswd{std::move(passwd)},
                            urtoken{std::move(rtoken)} {}
            
            std::int32_t GetID()     const {return uid;   }   
            std::string  GetName()   const {return uname;  } 
            std::string  GetPasswd() const {return upasswd;}
            std::string  GetRToken() const {return urtoken;}

            void SetName(std::string name)      {uname = std::move(name);    }  
            void SetPasswd(std::string passwd)  {upasswd= std::move(passwd); }
            void SetRToken(std::string rtoken)  {urtoken = std::move(rtoken);}
            
            auto Introspect() {return std::tie(uid,uname,upasswd,urtoken);}
        private:
        std::int32_t uid;
        std::string uname{};
        std::string upasswd{};
        std::string urtoken{};
    };
} // namespace models
