#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Account.h"
#include "Transaction.h"

class MockAccount : public Account
{
public:
    MockAccount(int id, int balance): Account(id, balance){}
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction : public Transaction
{
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};


using ::testing::AtLeast;
using ::testing::Return;
using testing::Ref;

TEST(TransactionTests, Check_set_fee)
{
    MockTransaction transaction;
    const int fee = 10;
    transaction.set_fee(fee);
    const int result = transaction.fee();
    EXPECT_EQ(fee, result) << "fee function returns wrong value";
}

TEST(TransactionTests, TrMakeBelowZero)
{
    MockAccount account1(1,100);
    MockAccount account2(2,150);
    MockTransaction transaction;
    const int sum = -5;

    EXPECT_CALL(transaction, SaveToDataBase(Ref(account1), Ref(account2), sum)).Times(0);
    EXPECT_THROW((transaction.Make(account1, account2, sum)), std::invalid_argument) 
            << "transaction.Make should raise exception when zero value passed"; 
}

TEST(TransactionTests, Tr_Make_From_and_To_Are_Same)
{
    MockAccount account1(1,100);
    MockAccount account2(2,150);
    MockTransaction transaction;
    const int sum = 150;

    EXPECT_CALL(transaction, SaveToDataBase(Ref(account1), Ref(account2), sum)).Times(0);
    EXPECT_THROW((transaction.Make(account1, account1, sum)), std::logic_error) 
            << "transaction.Make should raise exception when to and from accounts are equal"; 
}   

TEST(TransactionTests, Tr_Make_Fee_Larger_Than_sum)
{
    MockAccount account1(1,100);
    MockAccount account2(2,150);
    MockTransaction transaction;
    transaction.set_fee(100);
    const int sum = 110;

    EXPECT_CALL(transaction, SaveToDataBase(Ref(account1), Ref(account2), sum)).Times(0);
    
    const bool result = transaction.Make(account1, account2, sum);  
    EXPECT_FALSE(result); 
} 

TEST(TransactionTests, Tr_Succeeds)
{
    const int val1 = 150;
    const int val2 = 200;

    MockAccount account1(1,val1);
    MockAccount account2(2,val2);

    MockTransaction transaction;
    const int sum = 125;

    EXPECT_CALL(account1, Lock()).Times(1);
    EXPECT_CALL(account2, Lock()).Times(1);
    EXPECT_CALL(account2, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(account1, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(account1, GetBalance()).Times(1).WillOnce(Return(val1));
    EXPECT_CALL(account1, Unlock()).Times(1);
    EXPECT_CALL(account2, Unlock()).Times(1);
    EXPECT_CALL(transaction, SaveToDataBase(Ref(account1), Ref(account2), sum)).Times(1);

    const bool result = transaction.Make(account1, account2, sum);
    EXPECT_TRUE(result); 
}   

TEST(TransactionTests, Tr_not_succeeded)
{
    const int val1 = 150;
    const int val2 = 200;
    MockAccount account1(1,val1);
    MockAccount account2(2,val2);
    MockTransaction transaction;
    const int sum = 200;

    EXPECT_CALL(account1, Lock()).Times(1);
    EXPECT_CALL(account2, Lock()).Times(1);
    EXPECT_CALL(account2, ChangeBalance(sum)).Times(1);
    EXPECT_CALL(account2, ChangeBalance(-sum)).Times(1);
    EXPECT_CALL(account1, ChangeBalance(testing::_)).Times(0);
    EXPECT_CALL(account1, GetBalance()).Times(1).WillOnce(Return(val1));
    EXPECT_CALL(account1, Unlock()).Times(1);
    EXPECT_CALL(account2, Unlock()).Times(1);
    EXPECT_CALL(transaction, SaveToDataBase(Ref(account1), Ref(account2), sum)).Times(1); 
    //I dont't know if it really needs to save failed operation

    const bool result = transaction.Make(account1, account2, sum);
    EXPECT_FALSE(result); 
}   

TEST(TransactionTests, DatabaseSaving)
{
    const int val1 = 200;
    const int val2 = 250;
    MockAccount account1(1,val1);
    MockAccount account2(2,val2);
    Transaction transaction;
    const int sum = 150;

    EXPECT_CALL(account1, Lock()).Times(1);
    EXPECT_CALL(account2, Lock()).Times(1);
    EXPECT_CALL(account2, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(account1, ChangeBalance(testing::_)).Times(AtLeast(1));
    EXPECT_CALL(account1, GetBalance()).Times(2).WillOnce(Return(val1)).WillOnce(Return(val1 - sum - transaction.fee()));
    EXPECT_CALL(account2, GetBalance()).Times(1).WillOnce(Return(val2 + sum));
    EXPECT_CALL(account1, Unlock()).Times(1);
    EXPECT_CALL(account2, Unlock()).Times(1);
    
    testing::internal::CaptureStdout();
    transaction.Make(account1, account2, sum);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "1 send to 2 $150\nBalance 1 is 49\nBalance 2 is 400\n");

}
