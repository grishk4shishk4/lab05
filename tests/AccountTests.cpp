#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"


TEST(AccountTests, GetBalance)
{
    const int balance = 500;
    Account acc(1, balance);
    const int result = acc.GetBalance();
    EXPECT_EQ(balance, result);
}

TEST(AccountTests, ChangeBalance)
{
    const int balance = 850;
    const int sum = 150;
    Account acc(2, balance);
    acc.Lock();
    acc.ChangeBalance(sum);
    acc.Unlock();
    EXPECT_EQ((balance + sum), acc.GetBalance());
}

TEST(AccountTests, Lock_locked)
{
    const int balance = 1000;
    Account acc(3, balance);
    acc.Lock();
    EXPECT_THROW({acc.Lock();}, std::runtime_error);

}

TEST(AccountTests, ChangeBalance_unlocked)
{
    const int balance = 1000;
    Account acc(1, balance);
    acc.Unlock();   
    EXPECT_THROW({acc.ChangeBalance(100);}, std::runtime_error);
}
