#include <BaseApp.hpp> //precomp

#include <Network/DiscordBot.hpp>

DiscordBot g_discordBot;
DiscordBot* GetDiscordBot() { return &g_discordBot; }

void DiscordBot::Init()
{
    if (!GetBaseApp()->GetConfig().bUsingBot) {
        LogMsg("Bot usage is disabled in the configuration.");
        return;
    }
    if (m_pCluster) {
        LogMsg("Bot cluster is already initialized.");
        return;
    }

#ifdef _MSC_VER
    g_lastCall = __FUNCSIG__;
#else
    g_lastCall = __PRETTY_FUNCTION__;
#endif

    uint32_t intents = dpp::i_guilds | dpp::i_guild_messages | dpp::i_message_content;

    m_pCluster = new dpp::cluster(GetBaseApp()->GetConfig().botToken, intents);
    if (!m_pCluster)
        return;

    m_pCluster->on_interaction_create([&](const dpp::interaction_create_t& dppEvent) {
        if (dppEvent.command.get_command_name() == "stats") 
        {
            dpp::embed embed = dpp::embed().
                set_title("Server Uptime:").
                set_description(GetBaseApp()->GetUptime()).
                set_description("Players Online: " + GetOnlineManager()->GetPlayersOnline()).
                set_description("Active Worlds: " + GetWorldsManager()->GetWorlds().size()).
                set_color(std::rand() % 0xFFFFFF);

            dpp::message msg;
            msg.add_embed(embed);

            dppEvent.reply(dpp::ir_channel_message_with_source, msg);
        }
    });
    m_pCluster->on_message_create([&](const dpp::message_create_t& dppEvent) {});
    m_pCluster->on_button_click([&](const dpp::button_click_t& dppEvent) {});

    m_pCluster->on_ready([&](const dpp::ready_t& dppEvent) 
    {
        if (dpp::run_once<struct register_bot_commands>()) 
        {
            m_pCluster->set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "Waiting server..."));
        }

        dpp::slashcommand stats_command("stats", "Shows servers's statistics.", m_pCluster->me.id);
        m_pCluster->guild_command_create(stats_command, GetBaseApp()->GetConfig().guildID);

    });

    std::thread([this] { m_pCluster->start(false); }).detach();
    LogMsg("Discord bot is running.");
}