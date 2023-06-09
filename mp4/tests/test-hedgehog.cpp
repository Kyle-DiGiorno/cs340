// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/times.h>
// #include <sys/time.h>

// #include "../lib/wallet.h"
// #include "lib/catch.hpp"

// void *test_add_hedgehog_food(void *args) {
//   wallet_t *wallet = (wallet_t *) args;
//   for (int i = 0; i < 500; i++) {
//       wallet_change_resource(wallet, "hedgehog-food", 1);
//       //printf("\nhere migr\n");
//   }
//   //printf("\nhere retr\n");
//   return NULL;
// }

// void *test_add_hedgehogs(void *args) {
//   wallet_t *wallet = (wallet_t *) args;
//   for (int i = 0; i < 100; i++) {
//       wallet_change_resource(wallet, "hedgehog-food", -3);
//       //printf("\nmasterl\n");
//       wallet_change_resource(wallet, "hedgehogs", 1);
//       //printf("\nhernkm;l\n");
//   }
//   //printf("\nh0ipn\n");
//   return NULL;
// }

// TEST_CASE("hedgehog - 10 threads", "[weight=2][part=1]") {
//   clock_t start = clock();
//   time_t wall_begin, wall_end;
//   time(&wall_begin);

//   // Create and initialize wallets
//   wallet_t wallet;
//   wallet_init(&wallet);

//   pthread_t tids[10];
//   pthread_create(&tids[0], NULL, test_add_hedgehogs, &wallet);
//   pthread_create(&tids[1], NULL, test_add_hedgehogs, &wallet);
//   pthread_create(&tids[2], NULL, test_add_hedgehog_food, &wallet);
//   pthread_create(&tids[3], NULL, test_add_hedgehog_food, &wallet);
//   pthread_create(&tids[4], NULL, test_add_hedgehogs, &wallet);
//   pthread_create(&tids[5], NULL, test_add_hedgehogs, &wallet);
//   pthread_create(&tids[6], NULL, test_add_hedgehog_food, &wallet);
//   pthread_create(&tids[7], NULL, test_add_hedgehog_food, &wallet);
//   printf("\nhere gyhbnjr\n");
//   for (int i = 0; i < 8; i++) {
//     pthread_join(tids[i], NULL);
//     printf("\nhere thread%d\n",i);
//   }
//   printf("\nhere m in\n");
//   int num_food = wallet_get(&wallet, "hedgehog-food");
//   int num_hedgehogs = wallet_get(&wallet, "hedgehog-food");

//   // Destroy the wallets
//   wallet_destroy(&wallet);

//   clock_t end = clock();
//   time(&wall_end);

//   time_t wall_time = wall_end - wall_begin;
//   INFO("Wall Time: " <<  wall_time << " second(s)\n");

//   unsigned long cpu_clocks = end - start;
//   INFO("CPU Clocks: " << cpu_clocks << "\n");

//   REQUIRE(num_food == 800);
//   REQUIRE(num_hedgehogs == 800);
// }

// // Test whether proper conditional variables are being used to wait.
// void *test_add_hedgehog_food_mutex(void *args) {
//   wallet_t *wallet = (wallet_t *) args;
//   for (int i = 0; i < 5; i++) {
//     for (int j = 0; j < 100; j++) {
//       wallet_change_resource(wallet, "hedgehog-food", 1);
//     }
//     usleep(500);
//   }
//   return NULL;
// }

// void *test_add_hedgehogs_mutex(void *args) {
//   wallet_t *wallet = (wallet_t *) args;
//   for (int i = 0; i < 100; i++) {
//       wallet_change_resource(wallet, "hedgehog-food", -3);
//       wallet_change_resource(wallet, "hedgehogs", 1);
//   }
//   return NULL;
// }

// TEST_CASE("hedgehog - must NOT busy wait in wallet_change_resource", "[weight=10][part=2][timeout=30]") {
//   // Create and initialize wallets
//   clock_t start = clock();
//   time_t wall_begin, wall_end;
//   time(&wall_begin);

//   wallet_t wallet;
//   wallet_init(&wallet);

//   pthread_t tids[10];
//   pthread_create(&tids[0], NULL, test_add_hedgehogs_mutex, &wallet);
//   pthread_create(&tids[1], NULL, test_add_hedgehog_food_mutex, &wallet);

//   for (int i = 0; i < 2; i++) {
//     pthread_join(tids[i], NULL);
//   }

//   int num_food = wallet_get(&wallet, "hedgehog-food");
//   int num_hedgehogs = wallet_get(&wallet, "hedgehogs");

//   // Destroy the wallets
//   wallet_destroy(&wallet);

//   clock_t end = clock();
//   time(&wall_end);

//   // Require correct values of food and hedgehogs.
//   REQUIRE(num_food == 200);
//   REQUIRE(num_hedgehogs == 100);

//   time_t wall_time = wall_end - wall_begin;
//   INFO("Wall Time: " <<  wall_time << " second(s)\n");
//   REQUIRE(wall_time < 2);

//   unsigned long cpu_clocks = end - start;
//   INFO("CPU Clocks: " << cpu_clocks << "\n");
//   REQUIRE(cpu_clocks < 10000);
// }

// // Test whether proper conditional variables are being used to wait.
// void *test_add_hedgehog_food_mutex_fast(void *args) {
//   wallet_t *wallet = (wallet_t *) args;
//   for (int j = 500; j > 0; j--) {
//     wallet_change_resource(wallet, "hedgehog-food", 1);
//   }
//   return NULL;
// }

// TEST_CASE("hedgehog - must NOT sleep wait in wallet_change_resource", "[weight=10][part=2]") {
//   // Create and initialize wallets
//   clock_t start = clock();

//   wallet_t wallet;
//   wallet_init(&wallet);

//   pthread_t tids[10];
//   pthread_create(&tids[0], NULL, test_add_hedgehogs_mutex, &wallet);
//   pthread_create(&tids[1], NULL, test_add_hedgehog_food_mutex_fast, &wallet);

//   for (int i = 0; i < 2; i++) {
//     pthread_join(tids[i], NULL);
//   }

//   int num_food = wallet_get(&wallet, "hedgehog-food");
//   int num_hedgehogs = wallet_get(&wallet, "hedgehogs");

//   // Destroy the wallets
//   wallet_destroy(&wallet);

//   clock_t end = clock();

//   // Require correct values of food and hedgehogs.
//   REQUIRE(num_food == 200);
//   REQUIRE(num_hedgehogs == 100);

//   unsigned long cpu_clocks = end - start;
//   INFO("CPU Clocks: " << cpu_clocks << "\n");
//   REQUIRE(cpu_clocks < 40000);
// }
