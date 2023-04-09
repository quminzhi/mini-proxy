#include <gtest/gtest.h>

extern "C" {
#include <stdlib.h>
#include <proxy-module.h>
}

class ProxyModuleTest: public ::testing::Test {
protected:

  void SetUp() override {
  }

  void TearDown() override {}
};

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

TEST_F(ProxyModuleTest, TestParseRUI) {
  const char *uri0 = "http://www.google.com\n";
  char host[MAXLINE], port[MAXLINE], path[MAXLINE];
  int rc = 0;
  rc = parse_uri(uri0, host, port, path);
  EXPECT_EQ(strcmp(host, "www.google.com"), 0);
  EXPECT_EQ(strcmp(port, "80"), 0);
  EXPECT_EQ(strcmp(path, "/"), 0);

  const char *uri1 = "http://www.google.com:8080\n";
  rc = parse_uri(uri1, host, port, path);
  EXPECT_EQ(strcmp(host, "www.google.com"), 0);
  EXPECT_EQ(strcmp(port, "8080"), 0);
  EXPECT_EQ(strcmp(path, "/"), 0);

  const char *uri2 = "http://www.google.com:8080/index.html\n";
  rc = parse_uri(uri2, host, port, path);
  EXPECT_EQ(strcmp(host, "www.google.com"), 0);
  EXPECT_EQ(strcmp(port, "8080"), 0);
  EXPECT_EQ(strcmp(path, "/index.html"), 0);

  const char *uri3 = "http://www.google.com:8080/index.html";
  rc = parse_uri(uri3, host, port, path);
  EXPECT_EQ(strcmp(host, "www.google.com"), 0);
  EXPECT_EQ(strcmp(port, "8080"), 0);
  EXPECT_EQ(strcmp(path, "/index.html"), 0);

  const char *uri4 = "http://www.google.com:8080";
  rc = parse_uri(uri4, host, port, path);
  EXPECT_EQ(strcmp(host, "www.google.com"), 0);
  EXPECT_EQ(strcmp(port, "8080"), 0);
  EXPECT_EQ(strcmp(path, "/"), 0);

  const char *uri5 = "http://www.google.com";
  rc = parse_uri(uri5, host, port, path);
  EXPECT_EQ(strcmp(host, "www.google.com"), 0);
  EXPECT_EQ(strcmp(port, "80"), 0);
  EXPECT_EQ(strcmp(path, "/"), 0);

}
