# Makefile for the dynamic_array project.
#
#   make m1          milestone 1: its tests, your tests, and your da_add's allocation count
#   make m2          milestones 1 and 2: their tests, your tests, and make growth on yours
#   make m3          milestones 1 to 3: every test in da_test, and your tests
#   make m4          make m3, then make asan and make valgrind
#
#   make             build da_test and my_tests
#   make test        run every test in da_test, and your tests: the whole project
#                    (make test M=1 runs only milestone 1's tests, M=2 those up to
#                    milestone 2, and so on; make asan and make valgrind take M too)
#   make test-naive  run da_test on the naive array (it passes every test; it is only slow)
#   make growth      count what 100000 adds allocate, on yours and on the naive array
#   make bench       time both arrays as they grow
#   make demo        build particles_naive and particles: run each in a terminal
#   make game        build swarm_naive and swarm, a game: play each in a terminal
#   make asan        run the tests with AddressSanitizer, which reports memory errors
#   make valgrind    run the tests under Valgrind, which must report no errors and no leaks
#   make clean       delete everything make built
#
# Every program is linked with alloc_stats.o and WRAP, so every malloc, calloc, realloc
# and free goes through alloc_stats.c, which counts it.

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O0 -g
FAST_CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2
ASAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
	--track-origins=yes --error-exitcode=1
# make test, asan and valgrind run the tests up to milestone M, or all of them if M is unset.
M =
WRAP = -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc,--wrap=free
NAIVE = naive_dynamic_array.o

all: da_test my_tests

# The last line of make m1 .. make m4: $(call verdict,K) after the runs set status.
verdict = echo; \
	if [ $$status -eq 0 ]; then \
	  echo "MILESTONE $(1) COMPLETE"; \
	else \
	  echo "Milestone $(1) is not complete yet: fix the first problem above, then run make m$(1) again."; \
	  exit 1; \
	fi

dynamic_array.o: dynamic_array.c dynamic_array.h
	$(CC) $(CFLAGS) -c dynamic_array.c

alloc_stats.o: alloc_stats.c alloc_stats.h
	$(CC) $(CFLAGS) -c alloc_stats.c

da_test.o: da_test.c dynamic_array.h alloc_stats.h da_check.h
	$(CC) $(CFLAGS) -c da_test.c

my_tests.o: my_tests.c dynamic_array.h alloc_stats.h da_check.h
	$(CC) $(CFLAGS) -c my_tests.c

da_growth.o: da_growth.c dynamic_array.h alloc_stats.h da_check.h
	$(CC) $(CFLAGS) -c da_growth.c

da_test: da_test.o dynamic_array.o alloc_stats.o
	$(CC) $(CFLAGS) $(WRAP) -o $@ $^

my_tests: my_tests.o dynamic_array.o alloc_stats.o
	$(CC) $(CFLAGS) $(WRAP) -o $@ $^

da_test_naive: da_test.o $(NAIVE) alloc_stats.o
	$(CC) $(CFLAGS) $(WRAP) -o $@ $^

da_growth_yours: da_growth.o dynamic_array.o alloc_stats.o
	$(CC) $(CFLAGS) $(WRAP) -o $@ $^

da_growth_naive: da_growth.o $(NAIVE) alloc_stats.o
	$(CC) $(CFLAGS) $(WRAP) -o $@ $^

test: da_test my_tests
	@status=0; \
	MALLOC_PERTURB_=165 ./da_test $(M) || status=1; \
	echo; echo "my_tests.c (your tests)"; \
	MALLOC_PERTURB_=165 ./my_tests || status=1; \
	exit $$status

m1: da_test my_tests da_growth_yours
	@status=0; \
	MALLOC_PERTURB_=165 ./da_test 1 || status=1; \
	echo; echo "my_tests.c (your tests)"; \
	MALLOC_PERTURB_=165 ./my_tests || status=1; \
	echo; ./da_growth_yours info; \
	$(call verdict,1)

m2: da_test my_tests da_growth_yours
	@status=0; \
	MALLOC_PERTURB_=165 ./da_test 2 || status=1; \
	echo; echo "my_tests.c (your tests)"; \
	MALLOC_PERTURB_=165 ./my_tests || status=1; \
	echo; echo "make growth, on your array"; \
	./da_growth_yours || status=1; \
	$(call verdict,2)

m3: da_test my_tests
	@status=0; \
	MALLOC_PERTURB_=165 ./da_test 3 || status=1; \
	echo; echo "my_tests.c (your tests)"; \
	MALLOC_PERTURB_=165 ./my_tests || status=1; \
	$(call verdict,3)

m4: da_test my_tests da_test_asan my_tests_asan
	@status=0; \
	MALLOC_PERTURB_=165 ./da_test 4 || status=1; \
	echo; echo "my_tests.c (your tests)"; \
	MALLOC_PERTURB_=165 ./my_tests || status=1; \
	echo; echo "make asan"; \
	./da_test_asan 4 >/dev/null || { echo "make asan found a problem in da_test: see above, or run make asan"; status=1; }; \
	./my_tests_asan >/dev/null || { echo "make asan found a problem in my_tests: see above, or run make asan"; status=1; }; \
	echo; echo "make valgrind"; \
	$(VALGRIND) -q ./da_test 4 >/dev/null || { echo "make valgrind found a problem in da_test: see above, or run make valgrind"; status=1; }; \
	$(VALGRIND) -q ./my_tests >/dev/null || { echo "make valgrind found a problem in my_tests: see above, or run make valgrind"; status=1; }; \
	$(call verdict,4)

test-naive: da_test_naive
	@MALLOC_PERTURB_=165 ./da_test_naive

growth: da_growth_yours da_growth_naive
	@echo "== your array"; ./da_growth_yours; status=$$?; \
	echo; echo "== the naive array"; ./da_growth_naive; \
	exit $$status

bench_yours: bench.c dynamic_array.c dynamic_array.h alloc_stats.c alloc_stats.h
	$(CC) $(FAST_CFLAGS) $(WRAP) -DIMPL_NAME='"yours"' -o $@ bench.c dynamic_array.c alloc_stats.c

bench_naive: bench.c $(NAIVE) dynamic_array.h alloc_stats.c alloc_stats.h
	$(CC) $(FAST_CFLAGS) $(WRAP) -DIMPL_NAME='"naive"' -o $@ bench.c $(NAIVE) alloc_stats.c

bench: bench_naive bench_yours
	@./bench_naive; echo; ./bench_yours

DEMO_SRC = demos/main.c demos/demo.c demos/screen.c demos/particle_set.c demos/fountain.c \
	demos/flames.c demos/fireworks.c
DEMO_DEPS = $(DEMO_SRC) demos/demo.h demos/screen.h demos/particle_set.h dynamic_array.h alloc_stats.c alloc_stats.h

particles: $(DEMO_DEPS) dynamic_array.c
	$(CC) $(FAST_CFLAGS) $(WRAP) -DIMPL_NAME='"yours"' -o $@ $(DEMO_SRC) dynamic_array.c alloc_stats.c

particles_naive: $(DEMO_DEPS) $(NAIVE)
	$(CC) $(FAST_CFLAGS) $(WRAP) -DIMPL_NAME='"naive"' -o $@ $(DEMO_SRC) $(NAIVE) alloc_stats.c

demo: particles_naive particles
	@echo "Run ./particles_naive -fountain and ./particles -fountain, each in its own"
	@echo "terminal. Also try -flames, -fireworks, -rate N and -reserve guess|worst."

GAME_SRC = demos/swarm/main.c demos/swarm/play.c demos/swarm/game.c demos/swarm/geometry.c \
	demos/swarm/effects.c demos/swarm/map.c demos/swarm/ship.c demos/swarm/autopilot.c \
	demos/swarm/enemies.c demos/swarm/shots.c demos/swarm/powerups.c demos/swarm/draw.c \
	demos/swarm/hud.c demos/screen.c demos/particle_set.c
GAME_DEPS = $(GAME_SRC) demos/swarm/swarm.h demos/swarm/geometry.h demos/screen.h \
	demos/particle_set.h dynamic_array.h alloc_stats.c alloc_stats.h

swarm: $(GAME_DEPS) dynamic_array.c
	$(CC) $(FAST_CFLAGS) $(WRAP) -DIMPL_NAME='"yours"' -o $@ $(GAME_SRC) dynamic_array.c alloc_stats.c -lm

swarm_naive: $(GAME_DEPS) $(NAIVE)
	$(CC) $(FAST_CFLAGS) $(WRAP) -DIMPL_NAME='"naive"' -o $@ $(GAME_SRC) $(NAIVE) alloc_stats.c -lm

game: swarm_naive swarm
	@echo "Play ./swarm_naive, then ./swarm. Try starting on a high level."

da_test_asan: da_test.c dynamic_array.c dynamic_array.h alloc_stats.c alloc_stats.h da_check.h
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(WRAP) -o $@ da_test.c dynamic_array.c alloc_stats.c

my_tests_asan: my_tests.c dynamic_array.c dynamic_array.h alloc_stats.c alloc_stats.h da_check.h
	$(CC) $(CFLAGS) $(ASAN_FLAGS) $(WRAP) -o $@ my_tests.c dynamic_array.c alloc_stats.c

asan: da_test_asan my_tests_asan
	@status=0; \
	./da_test_asan $(M) || status=1; \
	./my_tests_asan || status=1; \
	exit $$status

valgrind: da_test my_tests
	@status=0; \
	$(VALGRIND) ./da_test $(M) || status=1; \
	$(VALGRIND) ./my_tests || status=1; \
	exit $$status

clean:
	rm -f dynamic_array.o alloc_stats.o da_test.o my_tests.o da_growth.o da_test my_tests \
	  da_test_naive da_growth_yours da_growth_naive bench_yours bench_naive particles \
	  particles_naive swarm swarm_naive da_test_asan my_tests_asan

.PHONY: all test m1 m2 m3 m4 test-naive growth bench demo game asan valgrind clean
