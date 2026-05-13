#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Account.h"

TEST(AccountTests, GetBalance)
{
	const int balance = 500;
	Account acc(01, balance);
	const int result = acc.GetBalance();
	EXPECT_EQ(balance, result);
}

TEST(AccountTests, ChangeBalance)
{
	const int balance = 850;
	const int deposit_sum = 150;
	Account acc(02, balance);
	acc.ChangeBalance(sum);
	EXPECT_EQ((balance + sum), acc.GetBalance());
}

TEST(AccountTests, Lock)
{
	Account acc(03, 1000);
	acc.Lock();
	EXPECT_THROW({acc.Lock();}, std::runtime_error);
}

