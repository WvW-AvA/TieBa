#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>
#include <json/json.h>

std::string time_to_str(std::chrono::system_clock::time_point time);

class Comment
{
private:
    int m_commentID;
    int m_userID;
    std::string m_content;
    int m_floor;
    int m_upvote;
    std::string m_date;

public:
#pragma region properties
    int get_userID()
    {
        return m_userID;
    }
    void set_userID(int value) { m_userID = value; }

    std::string get_content() { return m_content; }
    void set_content(const std::string &value)
    {
        m_content = value;
        set_date(std::chrono::system_clock::now());
    }

    int get_floor() { return m_floor; }
    void set_floor(int value) { m_floor = value; }

    int get_upvote() { return m_upvote; }
    void set_upvote(int value) { m_upvote = value; }

    std::string get_date() { return m_date; }
    void set_date(std::chrono::system_clock::time_point value) { m_date = time_to_str(value); }
    void set_date(const std::string &value) { m_date = value; }
#pragma endregion
    Json::Value to_json();
    Comment(int userID, const std::string &content);
    Comment(Json::Value value);
    ~Comment();
};

class PostNode
{
private:
    int m_userID;
    std::string m_content;
    int m_floor = 0;
    int m_upvote;
    std::string m_date;

public:
#pragma region Properties
    std::vector<Comment> comments;

    int get_userID() { return m_userID; }
    void set_userID(int value) { m_userID = value; }

    std::string get_content() { return m_content; }
    void set_content(const std::string &value)
    {
        m_content = value;
        set_date(std::chrono::system_clock::now());
    }

    int get_floor() { return m_floor; }
    void set_floor(int value) { m_floor = value; }

    int get_upvote() { return m_upvote; }
    void set_upvote(int value) { m_upvote = value; }

    std::string get_date() { return m_date; }
    void set_date(std::chrono::system_clock::time_point value) { m_date = time_to_str(value); }
    void set_date(const std::string &value) { m_date = value; }
#pragma endregion
    Json::Value to_json();
    void add_comment(int userID, std::string content);
    bool remove_commit(int floor);
    PostNode(int userID, const std::string &content);
    PostNode(Json::Value value);
    ~PostNode();
};

class Post
{
private:
    int m_userID;
    std::string m_title;
    int m_postID;
    int m_upvote;
    std::string m_date;

public:
#pragma region properties
    std::vector<PostNode> post_nodes;

    int get_post_owner() { return m_userID; }
    void set_post_owner(int value) { m_userID = value; }

    std::string get_title() { return m_title; }
    void set_title(const std::string &value)
    {
        m_title = value;
        set_date(std::chrono::system_clock::now());
    }

    int get_postID() { return m_postID; }
    void set_postID(int value) { m_postID = value; }

    int get_upvote() { return m_upvote; }
    void set_upvote(int value) { m_upvote = value; }

    std::string get_date() { return m_date; }
    void set_date(std::chrono::system_clock::time_point value) { m_date = time_to_str(value); }
    void set_date(const std::string &value) { m_date = value; }
#pragma endregion
    Json::Value to_json();
    void add_post_node(int userID, std::string content);
    bool remove_post_node(int floor);
    bool save(const std::string &path);
    Post(int userID, const std::string &title, const std::string &content);
    Post(const std::string &path);
    ~Post();
};