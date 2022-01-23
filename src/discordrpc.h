#ifndef DISCORDRPC_H
#define DISCORDRPC_H

#include <QString>
#include "discord_register.h"
#include "discord_rpc.h"
#include <Windows.h>
#include <ctime>

class DiscordRPC
{
    HMODULE m_hDiscordDLL;

    time_t start_time;
    QString current_build_name;
    QString current_component_name;

    using pDiscord_Initialize = void (*)(const char *applicationId,
                                         DiscordEventHandlers *handlers,
                                         int autoRegister,
                                         const char *optionalSteamId);
    using pDiscord_Shutdown = void (*)();
    using pDiscord_ClearPresence = void (*)();
    using pDiscord_UpdatePresence = void (*)(const DiscordRichPresence *presence);

    pDiscord_Initialize Discord_Initialize{};
    pDiscord_Shutdown Discord_Shutdown{};
    pDiscord_ClearPresence Discord_ClearPresence{};
    pDiscord_UpdatePresence Discord_UpdatePresence{};

public:
    void Init();
    void Deinit();

    void Update(QString data, QString type);
    void Update(QString data);

private:
    void Update();
};

extern DiscordRPC g_DiscordRPC;

#endif // DISCORDRPC_H
