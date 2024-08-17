#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

int Server::Kick(std::string &input, Client& c)
{
    std::string channelName;
    std::string userToKick;
    std::istringstream stream(input);
    stream >> channelName >> userToKick;  // Kanal adı ve kullanıcı adını ayır

    if (channelName.empty() || userToKick.empty())
    {
        Utilities::writeReply(c.cliFd, ERR_NEEDMOREPARAMS(c.nick, "KICK"));
        return 0;
    }

    if (channelName.size() <= 1 || channelName[0] != '#') {
        Utilities::writeReply(c.cliFd, ERR_NEEDMOREPARAMS(c.nick, "KICK"));
        return 0;
    }

    Channel *ch = getChannel(channelName);
    if (ch == NULL)
    {
        Utilities::writeReply(c.cliFd, ERR_NOSUCHCHANNEL(c.nick, channelName));
        return 0;
    }

    if (ch->oprt == NULL || c.user != ch->oprt->user)
    {
        Utilities::writeReply(c.cliFd, ERR_CHANOPRIVSNEEDED(c.nick, channelName));
        return 0;
    }

    bool userFound = false;
    for (ClientIterator it2 = ch->channel_client.begin(); it2 != ch->channel_client.end(); ++it2)
    {
        if (it2->nick == userToKick)
        {
            userFound = true;
            std::string msg = "";
            Utilities::writeReply(it2->cliFd, RPL_KICK(ch->oprt->nick, channelName, userToKick, msg));
            Utilities::writeReply(it2->cliFd, RPL_ENDOFNAMES(it2->nick, channelName));
            ch->channel_client.erase(it2);
            showRightGui(c, *ch);
            break;
        }
    }

    if (!userFound)
    {
        Utilities::writeReply(c.cliFd, ERR_USERNOTINCHANNEL(c.nick, userToKick, channelName));
    }

    return 0;
}
