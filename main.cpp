#include "model/model.hpp"
#include "iostream"
int main()
{
    Post post("../test1.json");
    post.remove_post_node(2);
    post.save("../test2.json");
}