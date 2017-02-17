#include <gtest/gtest.h>

extern "C" {
#include <data_fifo.h>
}

TEST(data_fifo, simple) {
    fifo_t my_fifo;
    EXPECT_EQ(fifo_init(&my_fifo, 3), 0);
    char data[] = {'1', '2'};
    EXPECT_EQ(fifo_write(&my_fifo, &data, 2), 2);
    char data2[] = {'3', '4'};
    EXPECT_EQ(fifo_write(&my_fifo, &data2, 1), 1);
    char out;
    EXPECT_EQ(my_fifo.full, 1);
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '1');
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '2');
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '3');
    EXPECT_EQ(my_fifo.full, 0);
    EXPECT_EQ(fifo_empty(&my_fifo), 1);
}

TEST(data_fifo, wrong_size) {
    fifo_t my_fifo;
    EXPECT_EQ(fifo_init(&my_fifo, 0), -1);
}

TEST(data_fifo, simple_overflow) {
    fifo_t my_fifo;
    EXPECT_EQ(fifo_init(&my_fifo, 3), 0);
    char data[] = {'1', '2'};
    EXPECT_EQ(fifo_write(&my_fifo, &data, 2), 2);
    char data2[] = {'3', '4'};
    EXPECT_EQ(fifo_write(&my_fifo, &data2, 1), 1);
    char out;
    EXPECT_EQ(my_fifo.full, 1);
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '1');
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '2');
    EXPECT_EQ(fifo_write(&my_fifo, &data2[1], 1), 1);
    EXPECT_EQ(my_fifo.full, 0);
    EXPECT_EQ(fifo_empty(&my_fifo), 0);
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '3');
    EXPECT_EQ(fifo_read(&my_fifo, &out, 1), 1);
    EXPECT_EQ(out, '4');
    EXPECT_EQ(my_fifo.full, 0);
    EXPECT_EQ(fifo_empty(&my_fifo), 1);
}