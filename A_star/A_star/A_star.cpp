#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct Node
{
    int x;
    int y;
    double g;
    double h;
    double f;
    Node* parent;

    Node(int x_val, int y_val, Node* partent_node = nullptr)
        : x(x_val), y(y_val), g(0.0), h(0.0), f(0.0), parent(partent_node) {}
};

double obliczHeurystyke(int x, int y, int cel_x, int cel_y) {
    return std::sqrt(std::pow(x - cel_x, 2) + std::pow(y - cel_y, 2));
}

int main()
{
    std::vector<Node*> wszystkieNodey;

    vector<vector<int>> grid;

    ifstream file("grid.txt");

    if (!file.is_open()) {
        cerr << "Nie udalo sie otworzyc pliku grid.txt\n";
        return 1;
    }

    int wartosc;

    for (int i = 0; i < 20; i++) {
        vector<int> wiersz;

        for (int j = 0; j < 20; j++) {
            file >> wartosc;
            wiersz.push_back(wartosc);
        }
        grid.push_back(wiersz);
    }

    file.close();

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }

    int cel_x = 19;
    int cel_y = 19;

    std::vector<Node*> listaOtwarta;

    std::vector<std::vector<bool>> listaZamknieta(20, std::vector<bool>(20, false));

    Node* startNode = new Node(0, 0);
    startNode->h = obliczHeurystyke(0, 0, cel_x, cel_y);
    startNode->f = startNode->g + startNode->h;

    listaOtwarta.push_back(startNode);
    wszystkieNodey.push_back(startNode);




    Node* celNode = nullptr;

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { 1 , -1, 0, 0 };

    while (!listaOtwarta.empty()) {
        int bestIdx = 0;
        for (int i = 1; i < listaOtwarta.size(); ++i) {
            if (listaOtwarta[i]->f <= listaOtwarta[bestIdx]->f) {
                bestIdx = i;
            }
        }

        Node* current = listaOtwarta[bestIdx];
        listaOtwarta.erase(listaOtwarta.begin() + bestIdx);

        if (current->x == cel_x && current->y == cel_y) {
            celNode = current;
            break;
        }

        listaZamknieta[current->y][current->x] = true;

        for (int i = 0; i < 4; ++i) {
            int sasiad_x = current->x + dx[i];
            int sasiad_y = current->y + dy[i];

            if (sasiad_x >= 0 && sasiad_x < 20 && sasiad_y >= 0 && sasiad_y < 20) {
                if (grid[19 - sasiad_y][sasiad_x] != 5) {
                    if (!listaZamknieta[sasiad_y][sasiad_x]) {
                        double nowy_koszt_g = current->g + 1.0;

                        bool na_liscie_otwartej = false;
                        Node* istniejacy_sasiad = nullptr;

                        for (Node* n : listaOtwarta) {
                            if (n->x == sasiad_x && n->y == sasiad_y) {
                                na_liscie_otwartej = true;
                                istniejacy_sasiad = n;
                                break;
                            }
                        }

                        if (!na_liscie_otwartej) {
                            Node* nowyNode = new Node(sasiad_x, sasiad_y, current);
                            
                            nowyNode->g = nowy_koszt_g;
                            nowyNode->h = obliczHeurystyke(sasiad_x, sasiad_y, cel_x, cel_y);
                            nowyNode->f = nowyNode->g + nowyNode->h;
                            
                            listaOtwarta.push_back(nowyNode);
                            wszystkieNodey.push_back(nowyNode);
                        }

                        else if (nowy_koszt_g < istniejacy_sasiad->g) {
                            istniejacy_sasiad->g = nowy_koszt_g;
                            istniejacy_sasiad->f = istniejacy_sasiad->g + istniejacy_sasiad->h;
                            istniejacy_sasiad->parent = current;
                        }
                    }
                }
            }
        }
    }

    if (celNode != nullptr) {
        Node* obecny = celNode;
        while (obecny != nullptr) {
            grid[19 - obecny->y][obecny->x] = 3;
            obecny = obecny->parent;
        }

        std::cout << "Optymalna trasa zostala znaleziona:" << std::endl;
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                std::cout << grid[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    else {
        std::cout << "Nie ma mozliwosci dotarcia do celu." << std::endl;
    }

    for (Node* wezel : wszystkieNodey) {
        delete wezel;
    }
    wszystkieNodey.clear();

    return 0;


}

