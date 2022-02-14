#include <iostream>
#include <fstream>
#include <exception>
#include "model.hpp"
#include "../global.hpp"
std::string time_to_str(std::chrono::system_clock::time_point time)
{
    auto t_t = std::chrono::system_clock::to_time_t(time);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t_t), "%F,%T");
    return ss.str();
}

#pragma region Comment Part
Comment::Comment(int userID, const std::string &content)
{
    set_userID(userID);
    set_content(content);
    set_date(std::chrono::system_clock::now());
}
Comment::Comment(Json::Value value)
{
    set_userID(value["UserID"].asInt64());
    set_content(value["CommentContent"].asCString());
    set_date(value["CommentDate"].asString());
    set_floor(value["CommentFloor"].asInt64());
    set_upvote(value["CommentUpvote"].asInt64());
}
Comment::~Comment() {}

Json::Value Comment::to_json()
{
    Json::Value self;
    self["UserID"] = get_userID();
    self["CommentContent"] = get_content();
    self["CommentDate"] = get_date();
    self["CommentFloor"] = get_floor();
    self["CommentUpvote"] = get_upvote();
    return self;
}
#pragma endregion

#pragma region PostNode Part
PostNode::PostNode(int userID, const std::string &content)
{
    set_userID(userID);
    set_content(content);
}
PostNode::PostNode(Json::Value value)
{
    set_userID(value["UserID"].asInt64());
    set_content(value["PostContent"].asCString());
    set_date(value["PostDate"].asCString());
    set_floor(value["PostFloor"].asInt64());
    set_upvote(value["PostUpvote"].asInt64());
    for (int i = 0; i < value["Comments"].size(); i++)
    {
        Comment cm(value["Comments"][i]);
        comments.push_back(cm);
    }
}
PostNode::~PostNode() {}

void PostNode::add_comment(int userID, std::string content)
{
    Comment c{userID, content};
    c.set_floor(comments.size());
    comments.push_back(c);
}

bool PostNode::remove_commit(int floor)
{
    if (floor < 0 || floor >= comments.size())
        return false;
    comments.erase(comments.begin() + floor);
    for (int i = 0; i < comments.size(); i++)
        comments[i].set_floor(i);
    return true;
}
Json::Value PostNode::to_json()
{
    Json::Value self;
    self["UserID"] = get_userID();
    self["PostContent"] = get_content();
    self["PostDate"] = get_date();
    self["PostFloor"] = get_floor();
    self["PostUpvote"] = get_upvote();
    for (int i = 0; i < comments.size(); i++)
        self["Comments"].append(comments[i].to_json());
    return self;
}
#pragma endregion

#pragma region Post Part
Post::Post(int userID, const std::string &title, const std::string &content)
{
    post_num++;
    set_postID(post_num);

    set_post_owner(userID);
    set_title(title);
    set_date(std::chrono::system_clock::now());
    add_post_node(userID, content);
}

Post::Post(const std::string &path)
{
    Json::Reader reader;
    Json::Value value;
    std::cout << path << std::endl;
    std::ifstream ifs(path, std::ios::binary);
    if (ifs.is_open() == false)
    {
        std::cout << "read json file error!\n";
        return;
    }

    if (reader.parse(ifs, value))
    {
        set_post_owner(value["PostOwner"].asInt64());
        set_postID(value["PostID"].asInt64());
        set_title(value["PostTitle"].asCString());
        set_date(value["Date"].asCString());
        set_upvote(value["Upvote"].asInt64());
        for (int i = 0; i < value["PostNodes"].size(); i++)
        {
            PostNode pn(value["PostNodes"][i]);
            post_nodes.push_back(pn);
        }
    }
    else
    {
        std::cout << "prase error!";
    }
    ifs.close();
}

Post::~Post()
{
}

void Post::add_post_node(int userID, std::string content)
{
    PostNode tem{userID, content};
    tem.set_floor(post_nodes.size());
    post_nodes.push_back(tem);
}

bool Post::remove_post_node(int floor)
{
    if (floor <= 0 || floor >= post_nodes.size())
        return false;
    post_nodes.erase(post_nodes.begin() + floor);
    for (int i = 0; i < post_nodes.size(); i++)
        post_nodes[i].set_floor(i);
    return true;
}

Json::Value Post::to_json()
{
    Json::Value self;
    self["PostTitle"] = get_title();
    self["PostTotal"] = post_nodes.size();
    self["PostOwner"] = get_post_owner();
    self["Date"] = get_date();
    self["Upvote"] = get_upvote();
    self["PostID"] = get_postID();
    for (int i = 0; i < post_nodes.size(); i++)
        self["PostNodes"].append(post_nodes[i].to_json());
    return self;
}

bool Post::save(const std::string &path)
{
    Json::StyledWriter writer;
    std::ofstream ofs{path, std::ios::binary};
    if (ofs.is_open() == false)
        return false;

    ofs << writer.write(to_json());
    ofs.close();
    return true;
}
#pragma endregion