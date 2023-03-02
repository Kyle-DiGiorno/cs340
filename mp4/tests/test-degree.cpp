// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/times.h>

// #include "../lib/wallet.h"
// #include "lib/catch.hpp"

// void *job_clover_patch(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   int i = 0;
//   for (i = 0; i < 100; i++)
//   {
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "four-leaf-clover", 1);
//     fprintf(stderr, "🍀");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     if (rand() % 10 == 0)
//     {
//       wallet_change_resource(wallet, "four-leaf-clover", 1);
//       fprintf(stderr, "🍀");
//     }
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//     wallet_change_resource(wallet, "clover", 1);
//     fprintf(stderr, "☘️");
//   }

//   return NULL;
// }

// void *job_orchard(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   int i = 0;
//   for (i = 0; i < 110; i++)
//   {
//     wallet_change_resource(wallet, "green-apple", 1);
//     fprintf(stderr, "🍏");
//   }

//   return NULL;
// }

// void *job_workshop(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   int i = 0;
//   for (i = 0; i < 520; i++)
//   {
//     wallet_change_resource(wallet, "tools", 1);
//     fprintf(stderr, "🧰");

//     if (i % 5 == 0)
//     {
//       wallet_change_resource(wallet, "gem", 1);
//       fprintf(stderr, "💎");
//     }
//   }

//   return NULL;
// }

// void *job_dna(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   int i = 0;
//   for (i = 0; i < 1750; i++)
//   {
//     wallet_change_resource(wallet, "dna", 1);
//     fprintf(stderr, "🧬");
//   }
//   //printf("\ndna%d\n",i);
//   return NULL;
// }

// void *job_research_green(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   // 📗 requires 1x🍏 1x🍀 10x☘️ 5x🧬
//   int i = 0;
//   for (i = 0; i < 100; i++)
//   {
//     wallet_change_resource(wallet, "green-apple", -1);
//     //printf("/ng 1\n");
//     wallet_change_resource(wallet, "four-leaf-clover", -1);
//     //printf("/ng 2\n");
//     wallet_change_resource(wallet, "clover", -10);
//     //printf("/ng 3\n");
//     wallet_change_resource(wallet, "dna", -5);
//     //printf("/ng 4\n");
//     wallet_change_resource(wallet, "green-book", 1);
//     //printf("/ng 5\n");
//     fprintf(stderr, "📗");
//   }
//   return NULL;
// }

// void *job_research_blue(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   // 📘 requires 10x🧬 1x💎
//   int i = 0;
//   for (i = 0; i < 100; i++)
//   {
//     wallet_change_resource(wallet, "dna", -10);
//     //printf("/nb 1\n");
//     wallet_change_resource(wallet, "gem", -1);
//     //printf("/nb 2\n");
//     wallet_change_resource(wallet, "blue-book", 1);
//     //printf("/nb 3\n");
//     fprintf(stderr, "📘");
//   }
//   return NULL;
// }

// void *job_research_orange(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   // 📙 requires 5x🧰 2x🧬
//   int i = 0;
//   for (i = 0; i < 100; i++)
//   {
//     wallet_change_resource(wallet, "tools", -5);
//     //printf("/no 1\n");
//     wallet_change_resource(wallet, "dna", -2);
//     //printf("/no 2\n");
//     wallet_change_resource(wallet, "orange-book", 1);
//     //printf("/no 3\n");
//     fprintf(stderr, "📙");
//   }
//   return NULL;
// }

// void *job_combine_research(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   // 📚 requires 1x📗, 1x📘, 1x📙
//   int i = 0;
//   for (i = 0; i < 100; i++)
//   {
//     wallet_change_resource(wallet, "orange-book", -1);
//     wallet_change_resource(wallet, "blue-book", -1);
//     wallet_change_resource(wallet, "green-book", -1);
//     wallet_change_resource(wallet, "books", 1);
    
//     fprintf(stderr, "📚");
//   }
//   return NULL;
// }

// void *job_graduation(void *args)
// {
//   wallet_t *wallet = (wallet_t *)args;
//   // 🎓 requires 100x 📚
//   wallet_change_resource(wallet, "books", -100);
//   wallet_change_resource(wallet, "degree!", 1);
//   fprintf(stderr, "🎓");
//   return NULL;
// }

// // Test 1
// TEST_CASE("degree", "[weight=5][part=1]")
// {
//   // Run multiple iterations of degree.
//   for (int it = 0; it < 2; it++)
//   {
//     wallet_t wallet;
//     wallet_init(&wallet);
//     srand(time(NULL));

//     // Require initial zero.
//     REQUIRE(wallet_get(&wallet, "degree!") == 0);
//     pthread_t tids[10];
//     pthread_create(&tids[0], NULL, job_graduation, (void *)&wallet);
//     pthread_create(&tids[1], NULL, job_combine_research, (void *)&wallet);
//     pthread_create(&tids[2], NULL, job_research_blue, (void *)&wallet);
//     pthread_create(&tids[3], NULL, job_research_orange, (void *)&wallet);
//     pthread_create(&tids[4], NULL, job_research_green, (void *)&wallet);
//     pthread_create(&tids[5], NULL, job_dna, (void *)&wallet);
//     pthread_create(&tids[6], NULL, job_workshop, (void *)&wallet);
//     pthread_create(&tids[7], NULL, job_clover_patch, (void *)&wallet);
//     pthread_create(&tids[8], NULL, job_orchard, (void *)&wallet);
//     const int totalJobs = 9;
//     for (int i = 0; i < totalJobs; i++)
//     {
//       printf("\nNew:\n");
//       int t1 = wallet_get(&wallet, "books");
//       int t2 = wallet_get(&wallet, "orange-book");
//       int t3 = wallet_get(&wallet, "blue-book");
//       int t4 = wallet_get(&wallet, "green-book");
//       int t5 = wallet_get(&wallet, "green-apple");
//       int t6 = wallet_get(&wallet, "dna");
//       int t7 = wallet_get(&wallet, "degree!");
//       pthread_join(tids[i], NULL);
//     }
//     printf("\nbooks-%d\n",wallet_get(&wallet, "books"));
//     printf("\norange-%d\n",wallet_get(&wallet, "orange-book"));
//     printf("\ngreen-%d\n",wallet_get(&wallet, "green-book"));
//     printf("\nblue-%d\n",wallet_get(&wallet, "blue-book"));
//     printf("\ngreen-apple-%d\n",wallet_get(&wallet, "green-apple"));
//     printf("\ndna%d\n",wallet_get(&wallet, "dna"));
//     REQUIRE(wallet_get(&wallet, "degree!") == 1);
//     wallet_destroy(&wallet);
//   }
// }
