#include <iostream>
#include <cmath>

class Polynomial {
private:
    struct Node {
        int coef; // 系數
        int exp;  // 指數
        Node* link; // 下一個節點
    };

    Node* head; // 頭節點

    void clear() {
        if (head) {
            Node* current = head->link;
            while (current != head) {
                Node* temp = current;
                current = current->link;
                delete temp;
            }
            delete head;
            head = nullptr;
        }
    }

public:
    Polynomial() : head(nullptr) {}

    // Copy Constructor
    Polynomial(const Polynomial& a) {
        head = nullptr;
        Node* current = a.head->link;
        do {
            addTerm(current->coef, current->exp);
            current = current->link;
        } while (current != a.head);
    }

    // Assignment Operator
    Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            clear();
            Node* current = a.head->link;
            do {
                addTerm(current->coef, current->exp);
                current = current->link;
            } while (current != a.head);
        }
        return *this;
    }

    // Destructor
    ~Polynomial() {
        clear();
    }

    void addTerm(int coef, int exp) {
        Node* newNode = new Node{ coef, exp, nullptr };
        if (!head) {
            head = newNode;
            head->link = head;
        }
        else {
            Node* current = head;
            while (current->link != head && current->link->exp > exp) {
                current = current->link;
            }
            newNode->link = current->link;
            current->link = newNode;
            if (exp > head->exp) {
                head = newNode;
            }
        }
    }

    float evaluate(float x) const {
        float result = 0;
        Node* current = head->link;
        do {
            result += current->coef * std::pow(x, current->exp);
            current = current->link;
        } while (current != head);
        return result;
    }

    // Addition
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* currentA = head->link;
        Node* currentB = b.head->link;

        while (currentA != head || currentB != b.head) {
            if (currentA->exp == currentB->exp) {
                result.addTerm(currentA->coef + currentB->coef, currentA->exp);
                currentA = currentA->link;
                currentB = currentB->link;
            }
            else if (currentA->exp > currentB->exp) {
                result.addTerm(currentA->coef, currentA->exp);
                currentA = currentA->link;
            }
            else {
                result.addTerm(currentB->coef, currentB->exp);
                currentB = currentB->link;
            }
        }
        return result;
    }

    // Stream Input
    friend std::istream& operator>>(std::istream& is, Polynomial& x) {
        int n;
        is >> n;
        for (int i = 0; i < n; ++i) {
            int coef, exp;
            is >> coef >> exp;
            x.addTerm(coef, exp);
        }
        return is;
    }

    // Stream Output
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& x) {
        Node* current = x.head->link;
        do {
            os << current->coef << "x^" << current->exp;
            if (current->link != x.head) os << " + ";
            current = current->link;
        } while (current != x.head);
        return os;
    }
};

int main() {
    Polynomial p1, p2;
    std::cout << "Enter the first polynomial (n coef1 exp1 coef2 exp2 ...): ";
    std::cin >> p1;
    std::cout << "Enter the second polynomial (n coef1 exp1 coef2 exp2 ...): ";
    std::cin >> p2;

    Polynomial sum = p1 + p2;
    std::cout << "Sum of polynomials: " << sum << std::endl;

    float x;
    std::cout << "Enter a value for x to evaluate: ";
    std::cin >> x;
    std::cout << "Evaluation of first polynomial: " << p1.evaluate(x) << std::endl;

    return 0;
}
