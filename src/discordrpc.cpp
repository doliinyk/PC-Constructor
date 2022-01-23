#include "discordrpc.h"
#include "singlecomponentwidget.h"
#include <cstring>

DiscordRPC g_DiscordRPC;

void DiscordRPC::Init()
{
    m_hDiscordDLL = LoadLibrary(TEXT("discord-rpc.dll"));

    Discord_Initialize = (pDiscord_Initialize) GetProcAddress(m_hDiscordDLL, "Discord_Initialize");
    Discord_Shutdown = (pDiscord_Shutdown) GetProcAddress(m_hDiscordDLL, "Discord_Shutdown");
    Discord_ClearPresence = (pDiscord_ClearPresence) GetProcAddress(m_hDiscordDLL,
                                                                    "Discord_ClearPresence");
    Discord_UpdatePresence = (pDiscord_UpdatePresence) GetProcAddress(m_hDiscordDLL,
                                                                      "Discord_UpdatePresence");

    DiscordEventHandlers nullHandlers{};
    Discord_Initialize("934515995568144384", &nullHandlers, 1, nullptr);

    start_time = time(nullptr);
}

void DiscordRPC::Deinit()
{
    Discord_ClearPresence();
    Discord_Shutdown();

    FreeLibrary(m_hDiscordDLL);
}

void DiscordRPC::Update(QString data, QString type)
{
    current_component_name = data;
    if (!data.isEmpty())
        current_component_name.prepend(SingleComponentWidget::translateComponentToText(type) + ": ");

    Update();
}

void DiscordRPC::Update(QString data)
{
    current_build_name = data;
    if (!data.isEmpty())
        current_build_name.prepend("Збірка: ");

    Update();
}

void DiscordRPC::Update()
{
    DiscordRichPresence presence{};

    presence.startTimestamp = start_time;
    presence.largeImageKey = "icon";
    presence.details = current_build_name.toStdString().c_str();
    presence.state = current_component_name.toStdString().c_str();

    Discord_UpdatePresence(&presence);
}
