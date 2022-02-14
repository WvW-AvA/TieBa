#include "model/model.hpp"
#include "iostream"
int main()
{
    Post post("../test.json");
    post.save("../test1.json");
}