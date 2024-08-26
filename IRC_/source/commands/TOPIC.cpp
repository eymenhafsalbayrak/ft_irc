#include "Server.hpp"
#include "Client.hpp"
#include "Utilities.hpp"
#include "Replies.hpp"
#include "Channel.hpp"

/*

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
ERR_CHANOPRIVSNEEDED (482)
RPL_NOTOPIC (331)
RPL_TOPIC (332)
RPL_TOPICWHOTIME (333)

*/

int Server::Topic(std::string &input, Client& c)
{
	std::vector<std::string> params = Utilities::splitString(input, ' ');

	if (params.size() < 1) {
		Utilities::writeReply(c.cliFd, ERR_NEEDMOREPARAMS(c.nick, "TOPIC"));
		return -1;
	}
	std::string channelName = params[0];
	std::string topicContent;
	if (params.size() > 1) {
		topicContent = Utilities::joinStrings(params.begin() + 1, params.end(), " ");
		if (!topicContent.empty() && topicContent[0] == ':') {
			topicContent.erase(0, 1);
		}
	}
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it).channel_name == channelName)
		{
			/* if((*it).topic.empty()){///check
				Utilities::writeReply(c.cliFd, RPL_NOTOPIC(c.nick, channelName));
			} */
			if((*it).topic_settable == false)
			{
				if((*it).channel_client[0].user == c.user)
				{
					//Utilities::writeReply(c.cliFd, RPL_TOPIC(c.nick, c.ipAddr, channelName, topicContent));
					(*it).topic = topicContent;
					std::cout <<"TOPIC : " << it->topic << std::endl;
					std::vector<int> fds = it->getFds();
					if (!fds.empty()) {
						Utilities::writeAllMessage(fds, RPL_TOPIC(c.nick, c.ipAddr, channelName, topicContent));
					}
				}
				else {
					Utilities::writeReply(c.cliFd, ERR_TOPICNOTSETTABLE(c.nick, channelName));
				}
				return 0;
			}
			else if ((*it).topic_settable == true)
			{
				//Utilities::writeReply(c.cliFd, RPL_TOPIC(c.nick, c.ipAddr, channelName, topicContent));
				(*it).topic = topicContent;
				std::cout <<"TOPIC : " << it->topic << std::endl;
				std::vector<int> fds = it->getFds();
				if (!fds.empty()) {
					Utilities::writeAllMessage(fds, RPL_TOPIC(c.nick, c.ipAddr, channelName, topicContent));
				}
				return 0;
			}
		}
		else { ///checkle
			Utilities::writeReply(c.cliFd, ERR_NOTONCHANNEL(c.nick, channelName));
			return 0;
		}
	}
	// Kanal bulunamadı
	Utilities::writeReply(c.cliFd, ERR_NOSUCHCHANNEL(c.nick, channelName));
	return -1;
}