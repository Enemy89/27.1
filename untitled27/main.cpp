#include <iostream>
#include <vector>
#include <string>
#include <ctime>

class Branch {
    Branch* parentBranch;
    std::vector<Branch*> subBranches;
    std::string name;

public:
    Branch(const std::string& n = "", Branch* parent = nullptr) : name(n), parentBranch(parent) {}

    void addSubBranch(Branch* subBranch) {
        subBranches.push_back(subBranch);
    }

    void generateBranches() {
        int numBigBranches = rand() % 3 + 3;

        for (int i = 0; i < numBigBranches; ++i) {
            std::cout << "Enter branch name (big) " << i + 1 << ": ";
            std::cin >> name;
            Branch* bigBranch = new Branch(name, this);
            subBranches.push_back(bigBranch);

            int numMidBranches = rand() % 2 + 2;

            for (int j = 0; j < numMidBranches; ++j) {
                std::cout << "Enter branch name (middle) " << j + 1 << ": ";
                std::cin >> name;
                Branch* midBranch = new Branch(name, bigBranch);
                bigBranch->addSubBranch(midBranch);
            }
        }
    }

    void deleteBranches() {
        for (int i = 0; i < subBranches.size(); ++i) {
            for (int j = 0; j < subBranches[i]->subBranches.size(); ++j) {
                delete subBranches[i]->subBranches[j];
            }
            delete subBranches[i];
        }
        subBranches.clear();
    }

    void printBranchStructure(int depth = 0) {
        for (int i = 0; i < subBranches.size(); ++i) {
            for (int k = 0; k < depth; ++k) {
                std::cout << "  "; // вывод отступов для визуального представления структуры
            }
            std::cout << "Branch: " << subBranches[i]->name << std::endl;

            subBranches[i]->printBranchStructure(depth + 1);
        }
    }

    std::string getParentName(const std::string& searchName) {
        for (int i = 0; i < subBranches.size(); ++i) {
            if (subBranches[i]->name == searchName) {
                return searchName;
            }
            for (int j = 0; j < subBranches[i]->subBranches.size(); ++j) {
                if (subBranches[i]->subBranches[j]->name == searchName) {
                    return subBranches[i]->name;
                }
            }
        }
        return "";
    }

    int countMiddleBranches(const std::string& searchName, Branch* root) {
        std::string parentName = getParentName(searchName);

        if (parentName != "") {
            int count = 0;
            for (int i = 0; i < root->subBranches.size(); ++i) {
                if (root->subBranches[i]->name == parentName) {
                    for (int j = 0; j < root->subBranches[i]->subBranches.size(); ++j) {
                        if (root->subBranches[i]->subBranches[j]->name != "None" &&
                            root->subBranches[i]->subBranches[j]->name != searchName) {
                            count++;
                        }
                    }
                }
            }
            return count;
        }
    }
};

int main() {
    std::srand(std::time(nullptr));

    Branch* trees[2];

    for (int i = 0; i < 2; ++i) {
        trees[i] = new Branch();
        std::cout << "Tree #" << i + 1 << std::endl;
        trees[i]->generateBranches();
    }

    // Вывод структуры деревьев
    for (int i = 0; i < 2; ++i) {
        std::cout << "Tree " << i + 1 << " structure:" << std::endl;
        trees[i]->printBranchStructure();
        std::cout << std::endl;
    }

    std::string searchName;
    std::cout << "Enter elf name to search: ";
    std::cin >> searchName;

    int count = 0;
    for (int i = 0; i < 2; ++i) {
        int branchesCount = trees[i]->countMiddleBranches(searchName, trees[i]);
        if (branchesCount != -1) {
            count += branchesCount;
        }
    }

    if (count != 0) {
        std::cout << "Total number of neighbors: " << count << std::endl;
    } else {
        std::cout << "Big branches not found or do not contain middle branches!" << std::endl;
    }

    // Освобождение памяти
    for (int i = 0; i < 2; ++i) {
        trees[i]->deleteBranches();
        delete trees[i];
    }
}