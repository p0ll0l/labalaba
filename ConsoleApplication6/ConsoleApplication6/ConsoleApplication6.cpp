#include <iostream>
#include <vector>

int findFakeCoinPosition(std::vector<int>& coins) {
    int left = 0;
    int right = coins.size() - 1;

    while (left < right) {
        int mid = (left + right) / 2;

        // Считаем сумму элементов в левой и правой частях
        int leftSum = 0;
        for (int i = left; i < mid; ++i) {
            leftSum += coins[i];
        }

        int rightSum = 0;
        for (int i = mid; i <= right; ++i) {
            rightSum += coins[i];
        }

        if (leftSum > rightSum) {
            right = mid - 1;
        }
        else if (leftSum < rightSum) {
            left = mid + 1;
        }
        else {
            return mid;
        }
    }

    return left;
}

int main() {
    std::vector<int> coins = { 0, 0, 1 };
    int fakeCoinIndex = findFakeCoinPosition(coins);
    std::cout << "Позиция фальшивой монеты: " << fakeCoinIndex << std::endl;

    return 0;
}