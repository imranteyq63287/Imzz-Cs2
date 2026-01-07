#include "../includes.h"
#include <future>
class c_authorization {
public:
	bool LoadCredentialsFromFile(std::string& login, std::string& password, std::string& uid);
	bool LoadCredentials(std::string& login, std::string& password, std::string& uid);
	bool CheckCredentialsMatch();
	void send_identity(const std::string& steamid = std::to_string(g_ctx.local_controller->steam_id()), const std::string& nickname = g_ctx.local_controller->player_name());	
	std::future<std::unordered_map<std::string, std::string>> load_known_players();
	std::string username;
	std::string password;
	std::string uid;
	std::future<std::unordered_map<std::string, std::string>> future;
	std::unordered_map<std::string, std::string> cached_known_players;


};

inline const auto g_authorization = std::make_unique<c_authorization>();