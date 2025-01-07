#include <iostream>
#include <cmath> // For pow()
using namespace std;

// 節點結構 (Node)
struct Node {
    int coef; // 係數
    int exp;  // 指數
    Node* next; // 指向下一個節點

    Node(int c = 0, int e = 0, Node* n = nullptr) : coef(c), exp(e), next(n) {}
};

// 多項式類別 (Polynomial)
class Polynomial {
private:
    Node* header; // 環形鏈結串列的頭節點

    // 清除所有節點
    void clear() {
        if (!header) return;
        Node* current = header->next;
        while (current != header) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        delete header;
        header = nullptr;
    }

public:
    // 預設建構子
    Polynomial() {
        header = new Node(); // 建立頭節點
        header->next = header; // 環狀
    }

    // 複製建構子
    Polynomial(const Polynomial& other) {
        header = new Node();
        header->next = header;

        Node* current = other.header->next;
        while (current != other.header) {
            addTerm(current->coef, current->exp);
            current = current->next;
        }
    }

    // 解構子
    ~Polynomial() {
        clear();
    }

    // 複製賦值運算子
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            clear();
            Node* current = other.header->next;
            while (current != other.header) {
                addTerm(current->coef, current->exp);
                current = current->next;
            }
        }
        return *this;
    }

    // 加入一項 (依據指數順序加入)
    void addTerm(int coef, int exp) {
        if (coef == 0) return;

        Node* prev = header;
        Node* current = header->next;

        // 找到適當的位置插入 (降序排列)
        while (current != header && current->exp > exp) {
            prev = current;
            current = current->next;
        }

        if (current != header && current->exp == exp) {
            // 合併同指數項
            current->coef += coef;
            if (current->coef == 0) { // 若係數為 0，刪除該節點
                prev->next = current->next;
                delete current;
            }
        }
        else {
            // 插入新節點
            Node* newNode = new Node(coef, exp, current);
            prev->next = newNode;
        }
    }

    // 輸入運算符 >>
    friend istream& operator>>(istream& is, Polynomial& poly) {
        int n, coef, exp;
        is >> n; // 讀取多項式的項數
        for (int i = 0; i < n; ++i) {
            is >> coef >> exp;
            poly.addTerm(coef, exp);
        }
        return is;
    }

    // 輸出運算符 <<
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        Node* current = poly.header->next;
        bool first = true;

        while (current != poly.header) {
            if (!first && current->coef > 0) os << " + ";
            if (current->coef < 0) os << " - ";

            os << abs(current->coef);
            if (current->exp != 0) os << "x^" << current->exp;

            current = current->next;
            first = false;
        }

        if (first) os << "0"; // 空多項式
        return os;
    }

    // 多項式相加
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        Node* current = header->next;
        Node* otherCurrent = other.header->next;

        while (current != header || otherCurrent != other.header) {
            if (current != header && (otherCurrent == other.header || current->exp > otherCurrent->exp)) {
                result.addTerm(current->coef, current->exp);
                current = current->next;
            }
            else if (otherCurrent != other.header && (current == header || current->exp < otherCurrent->exp)) {
                result.addTerm(otherCurrent->coef, otherCurrent->exp);
                otherCurrent = otherCurrent->next;
            }
            else {
                result.addTerm(current->coef + otherCurrent->coef, current->exp);
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }
        return result;
    }

    // 多項式相減
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;
        Node* current = header->next;
        Node* otherCurrent = other.header->next;

        while (current != header || otherCurrent != other.header) {
            if (current != header && (otherCurrent == other.header || current->exp > otherCurrent->exp)) {
                result.addTerm(current->coef, current->exp);
                current = current->next;
            }
            else if (otherCurrent != other.header && (current == header || current->exp < otherCurrent->exp)) {
                result.addTerm(-otherCurrent->coef, otherCurrent->exp);
                otherCurrent = otherCurrent->next;
            }
            else {
                result.addTerm(current->coef - otherCurrent->coef, current->exp);
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }

        return result;
    }

    // 多項式相乘
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        Node* current = header->next;

        while (current != header) {
            Node* otherCurrent = other.header->next;
            while (otherCurrent != other.header) {
                result.addTerm(current->coef * otherCurrent->coef, current->exp + otherCurrent->exp);
                otherCurrent = otherCurrent->next;
            }
            current = current->next;
        }

        return result;
    }

    // 評估多項式值
    float Evaluate(float x) const {
        float result = 0;
        Node* current = header->next;
        while (current != header) {
            result += current->coef * pow(x, current->exp);
            current = current->next;
        }
        return result;
    }
};

// 主程式
int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式的項數與係數和指數 : ";
    cin >> p1;
    cout << "輸入第二個多項式的項數與係數和指數 : ";
    cin >> p2;

    cout << "第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;

    Polynomial sum = p1 + p2;
    cout << "相加結果: " << sum << endl;

    Polynomial diff = p1 - p2;
    cout << "相減結果: " << diff << endl;

    Polynomial prod = p1 * p2;
    cout << "相乘結果: " << prod << endl;

    float x;
    cout << "輸入 x 的值以評估第一個多項式: ";
    cin >> x;
    cout << "評估結果: " << p1.Evaluate(x) << endl;

    return 0;
}
