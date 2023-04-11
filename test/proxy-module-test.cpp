#include <gtest/gtest.h>

extern "C" {
#include <stdlib.h>
#include <proxy-module.h>
}

/* MUST: define agent request header info which is referenced by proxy-module */
const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";
const char *accept_hdr =
    "Accept: "
    "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
const char *connection_hdr = "Connection: close\r\n";
const char *proxy_connection_hdr = "Proxy-Connection: close\r\n\r\n";

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
