#include <iostream>
#include <string>
#include <ctime>
#include <vector>

class Branch {
    Branch* parent;
    Branch** branches;
    int numBranches;
    std::string elf;

public:
    Branch(Branch* p, int num) : parent(p), numBranches(num), elf("") {
        branches = new Branch*[numBranches];
        for (int i = 0; i < numBranches; ++i) {
            branches[i] = new Branch(this, rand() % 2 + 2); // Генерация от 2 до 3 средних ветвей на каждой большой ветви
        }
    }

    void addElf(const std::string& elfName) {
        if (elf.empty()) {
            elf = elfName;
        } else {
            for (int i = 0; i < numBranches; ++i) {
                branches[i]->addElf(elfName);
            }
        }
    }

    int countElvesOnSameBranch(const std::string& name) {
        int count = 0;
        std::vector<Branch*> nodesToProcess;
        nodesToProcess.push_back(this);

        while (!nodesToProcess.empty()) {
            Branch* current = nodesToProcess.back();
            nodesToProcess.pop_back();

            if (current->elf == name) {
                count++;
            }

            for (int i = 0; i < current->numBranches; ++i) {
                if (current->branches[i] != nullptr) {
                    nodesToProcess.push_back(current->branches[i]);
                }
            }
        }

        return count;
    }

    void generateBranches() {
        int numBigBranches = rand() % 3 + 3; // Генерация от 3 до 5 больших ветвей

        branches = new Branch*[numBigBranches];
        for (int i = 0; i < numBigBranches; ++i) {
            branches[i] = new Branch(this, rand() % 2 + 2); // Генерация от 2 до 3 средних ветвей на каждой большой ветви
        }
    }

    Branch* findElf(const std::string& name) {
        if (elf == name) {
            return this;
        }

        Branch* found = nullptr;
        for (int i = 0; i < numBranches && found == nullptr; ++i) {
            found = branches[i]->findElf(name);
        }

        return found;
    }

    void deleteBranches() {
        for (int i = 0; i < numBranches; ++i) {
            if (branches[i] != nullptr) {
                branches[i]->deleteBranches();
                delete branches[i];
            }
        }
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Branch* trees[5];

    for (int i = 0; i < 5; ++i) {
        trees[i] = new Branch(nullptr, rand() % 3 + 3);
        trees[i]->generateBranches();
    }

    std::string elfName;
    while (true) {
        std::cout << "Enter elf name (or None): ";
        std::cin >> elfName;

        if (elfName == "exit") {
            break;
        }

        for (int i = 0; i < 5; ++i) {
            trees[i]->addElf(elfName);
        }
    }

    std::cout << "Enter elf name to find: ";
    std::string findElfName;
    std::cin >> findElfName;

    int totalNeighbors = 0;
    for (int i = 0; i < 5; ++i) {
        Branch* found = trees[i]->findElf(findElfName);
        if (found != nullptr) {
            totalNeighbors += trees[i]->countElvesOnSameBranch(findElfName) - 1;
        }
    }

    std::cout << "Total neighbors for " << findElfName << ": " << totalNeighbors << std::endl;

    for (int i = 0; i < 5; ++i) {
        if (trees[i] != nullptr) {
            trees[i]->deleteBranches();
            delete trees[i];
            trees[i] = nullptr;
        }
    }
}