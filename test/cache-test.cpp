#include <cstring>
#include <gtest/gtest.h>
#include <string>

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
  char *host = "http://www.google.com";
  char *port = "80";
  char *path = "/developer/hello.cpp";
  char *payload = "232kj3jk232nmnmasbcbuewei";
  size_t bytesize = 1;
  cnode_t *node = new_node(host, port, path, payload, bytesize);
  enqueue(node);

  ASSERT_TRUE(front->next == node);
  ASSERT_TRUE(node == end->prev);
  ASSERT_TRUE(is_empty() == 0);
  ASSERT_TRUE(cache_count == 1);
  ASSERT_EQ(cache_load, strlen(payload));
}

TEST_F(CacheTest, TestDequeue) {
  char *host = "https://www.google.com";
  char *port = "443";
  char *path = "/developer/hello.cpp";
  char *payload = "232kj3jk232nmnmasbcbuewei";
  size_t bytesize = 2;
  cnode_t *node = new_node(host, port, path, payload, bytesize);
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

TEST_F(CacheTest, TestLRUPolicy) {
  char *host1 = "http://www.google.com";
  char *port1 = "80";
  char *path1 = "/developer/hello.cpp";
  char *payload1 = "232kj3jewei";
  size_t bytesize1 = 12;
  cnode_t *node1 = new_node(host1, port1, path1, payload1, bytesize1);
  enqueue(node1);
  char *host2 = "https://www.windows.com";
  char *port2 = "443";
  char *path2 = "/developer/hello.cpp";
  char *payload2 = "232k";
  size_t bytesize2 = 12;
  cnode_t *node2 = new_node(host2, port2, path2, payload2, bytesize2);
  enqueue(node2);
  char *host3 = "https://www.google.com";
  char *port3 = "443";
  char *path3 = "/developer/home.html";
  char *payload3 = "232kj3sssk232nmnmasbcbuewei";
  size_t bytesize3 = 12;
  cnode_t *node3 = new_node(host3, port3, path3, payload3, bytesize3);
  enqueue(node3);

  cnode_t *last1 = end->prev;
  ASSERT_TRUE(strcmp(last1->host, host3) == 0);
  cnode_t *last2 = last1->prev;
  ASSERT_TRUE(strcmp(last2->host, host2) == 0);
  cnode_t *last3 = last2->prev;
  ASSERT_TRUE(strcmp(last3->host, host1) == 0);

  update(node2);
  last1 = end->prev;
  ASSERT_TRUE(strcmp(last1->host, host2) == 0);
  last2 = last1->prev;
  ASSERT_TRUE(strcmp(last2->host, host3) == 0);
  last3 = last2->prev;
  ASSERT_TRUE(strcmp(last3->host, host1) == 0);

  ASSERT_EQ(cache_count, 3);
  ;
  size_t total_payload =
      strlen(node1->payload) + strlen(node2->payload) + strlen(node3->payload);
  ASSERT_EQ(cache_load, total_payload);
}

TEST_F(CacheTest, TestIsCached) {
  char *host1 = "http://www.google.com";
  char *port1 = "80";
  char *path1 = "/developer/hello.cpp";
  char *payload1 = "232kj3jewei";
  size_t bytesize1 = 1;
  add_cache(host1, port1, path1, payload1, bytesize1);
  char *host2 = "https://www.windows.com";
  char *port2 = "443";
  char *path2 = "/developer/hello.cpp";
  char *payload2 = "232k";
  size_t bytesize2 = 1;
  add_cache(host2, port2, path2, payload2, bytesize2);
  char *host3 = "https://www.google.com";
  char *port3 = "443";
  char *path3 = "/developer/home.html";
  char *payload3 = "232kj3sssk232nmnmasbcbuewei";
  size_t bytesize3 = 1;
  add_cache(host3, port3, path3, payload3, bytesize3);

  char payload_cache[MAX_OBJECT_SIZE];
  size_t *bytesize;
  int rc = is_cached(host1, port1, path1, payload_cache, bytesize);
  ASSERT_TRUE(rc == 1);
  ASSERT_EQ(strcmp(payload_cache, payload1), 0);
}
