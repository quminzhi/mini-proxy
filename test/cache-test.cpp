#include <gtest/gtest.h>
#include <string.h>

extern "C" {
#include <cache.h>
#include <stdlib.h>
}

class CacheTest : public ::testing::Test {
protected:
  void SetUp() override { cache_init(); }

  void TearDown() override { cache_destroy(); }
};

#define GTEST_COUT std::cerr << "[          ] [ INFO ]"

TEST_F(CacheTest, TestCacheQueueInitialization) {
  ASSERT_TRUE(front->next == end);
  ASSERT_TRUE(front == end->prev);
  ASSERT_TRUE(is_empty() == 1);
}

TEST_F(CacheTest, TestEnqueue) {
  char *host = "https://www.google.com";
  char *port = "80";
  char *path = "/developer/hello.cpp";
  char *payload = "232kj3jk232nmnmasbcbuewei";
  cnode_t *node = new_node(host, port, path, payload);
  enqueue(node);

  ASSERT_TRUE(front->next == node);
  ASSERT_TRUE(node == end->prev);
  ASSERT_TRUE(is_empty() == 0);
  ASSERT_TRUE(cache_count == 1);
  ASSERT_EQ(cache_load, strlen(payload));
}

TEST_F(CacheTest, TestDequeue) {
  char *host = "https://www.google.com";
  char *port = "80";
  char *path = "/developer/hello.cpp";
  char *payload = "232kj3jk232nmnmasbcbuewei";
  cnode_t *node = new_node(host, port, path, payload);
  enqueue(node);
  cnode_t *del = dequeue();
  ASSERT_TRUE(strcmp(del->host, host) == 0);
  ASSERT_TRUE(strcmp(del->port, port) == 0);
  ASSERT_TRUE(strcmp(del->path, path) == 0);
  ASSERT_TRUE(strcmp(del->payload, payload) == 0);
  free_node(del);
  ASSERT_EQ(cache_count, 0);
  ASSERT_EQ(cache_load, 0);
}
