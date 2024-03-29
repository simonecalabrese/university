/* One-variable polynomials */

#include <iostream>

using std::cin, std::cout, std::string;

bool isNumeric(const string s) {
    for(int i = 0; i < s.length(); i++) {
        if(!isdigit(s[i]) && s[i] != '.' && s[i] != '+' && s[i] != '-') return false;
    }
    return true;
}

struct Monomial {
    double coefficient;
    char variable;
    double power;
};

class Polynomial {
    public:

        Monomial *monomials;
        int terms; // total terms

        /* Ask the user to build a Polynomial */
        Polynomial() {
            cout << "\nPolynomial construction.\n";
            cout << "How many monomials do you want to insert? ";
            string input;
            cin >> input;
            if(isNumeric(input) && stoi(input) > 0) {
                terms = stoi(input);
                // start to build up the polynomial
                this->monomials = new Monomial[terms];
                if(this->monomials == nullptr) {
                    cout << "Memory is full.\n";
                    exit(137);
                }
                buildpolynomial:
                for(int i = 0; i < terms; i++) {
                    Monomial m = {0,0,0};
                    cout << "Monomial " << i + 1 << "\n";
                    cout << "Coefficient: ";
                    cin >> input;
                    if(isNumeric(input)) {
                        m.coefficient = stod(input);
                        cout << "Variable symbol or 0 for coefficient only: ";
                        cin >> input;
                        if(input.compare("0") == 0) {
                            // check if a term with the same variable (constant) already exists
                            int found = this->getMonomial(0, 0);
                            if(found == -1) this->monomials[i] = m;
                            else {
                                // add to the term
                                this->monomials[found].coefficient += m.coefficient;
                            }
                        } else {
                            if(!isNumeric(input)) {
                                m.variable = input[0];
                                cout << "Power: ";
                                cin >> input;
                                if(isNumeric(input)) {
                                    m.power = stod(input);
                                    // check if a term with the same variable and power already exists 
                                    int found = this->getMonomial(m.variable, m.power);
                                    if(found == -1) this->monomials[i] = m;
                                    else {
                                        // add to the term
                                        this->monomials[found].coefficient += m.coefficient;
                                    }
                                } else {
                                    cout << "Invalid number.\n";
                                    goto buildpolynomial;
                                }
                            } else {
                                cout << "You can write only one alphabetical variable.\n";
                                goto buildpolynomial;
                            }
                        }
                    } else {
                        cout << "Invalid number.\n";
                        goto buildpolynomial;
                    }
                }
            } else {
                cout << "Invalid number.\n";
            }
        }

        /* Instantiate monomials array with a certain number of `terms`  */
        Polynomial(const int terms) {
            this->terms = terms;
            monomials = new Monomial[terms];
        }

        /* Return the highest of the degrees of the polynomial's monomials  */
        double degree() {
            if(monomials == nullptr) return 0;
            Monomial max;
            if(monomials[0].variable != 0) max = monomials[0];
            for(int i = 0; i < terms; i++) {
                if(monomials[i].variable == max.variable && monomials[i].power > max.power) {
                    max = monomials[i];
                }
            }
            return max.power;
        }

        /* Return the sum of all the monomials with a specific `variable` and `power` */
        double getMonomialSum(char variable, double power) {
            if(monomials == nullptr) return 0;
            double sum = 0;
            for(int i = 0; i < terms; i++) {
                if(monomials[i].variable == variable && monomials[i].power == power) {
                    sum += monomials[i].coefficient;
                }
            }
            return sum;
        }

        /* Return the index of a term with a specific `variable` and power if exists inside the polynomial */
        int getMonomial(const double variable, const double power) {
            for(int i = 0; i < terms; i++) {
                if(monomials[i].variable == variable && monomials[i].coefficient != 0 && monomials[i].power == power) return i;
            }
            return -1;
        }

        friend std::ostream &operator<<(std::ostream &os, Polynomial &p) {
            if(p.monomials == nullptr) return os;
            for(int i = 0; i < p.terms; i++) {
                if(p.monomials[i].coefficient != 0) {
                    if(p.monomials[i].variable != 0)
                        os << (p.monomials[i].coefficient >= 0 ? "+" : "") << p.monomials[i].coefficient << p.monomials[i].variable << "^" << p.monomials[i].power << " ";
                    else
                        os << (p.monomials[i].coefficient >= 0 ? "+" : "") << p.monomials[i].coefficient << " ";
                }
            }
            return os;
        } 
        
        friend bool operator>(Polynomial &a, Polynomial &b) {
            return a.degree() > b.degree();
        }

        friend Polynomial operator+(Polynomial &a, Polynomial &b) {
            if(a.monomials == nullptr) return b;
            if(b.monomials == nullptr) return a;
            int sumPolynomialLength = abs(a.terms - b.terms); 
            Polynomial sum = Polynomial((a.terms > b.terms) ? a.terms+sumPolynomialLength : b.terms+sumPolynomialLength);
            if(a.terms >= b.terms) {
                int i = 0;
                for(; i < a.terms; i++) {
                    sum.monomials[i].variable = a.monomials[i].variable;
                    sum.monomials[i].power = a.monomials[i].power;
                    sum.monomials[i].coefficient = a.monomials[i].coefficient;
                    sum.monomials[i].coefficient += b.getMonomialSum(a.monomials[i].variable, a.monomials[i].power);
                }
                i++;
                for(int j = 0; j < b.terms; j++) {
                    // check if a term with a certain variable already exist inside a, otherwise add it to the sum
                    if(sum.getMonomial(b.monomials[j].variable, b.monomials[j].power) == -1) {
                        sum.monomials[i] = b.monomials[j];
                    }
                }
            } else {
                int i = 0;
                for(; i < b.terms; i++) {
                    sum.monomials[i].variable = b.monomials[i].variable;
                    sum.monomials[i].power = b.monomials[i].power;
                    sum.monomials[i].coefficient = b.monomials[i].coefficient;
                    sum.monomials[i].coefficient += a.getMonomialSum(b.monomials[i].variable, b.monomials[i].power);
                }
                i++;
                for(int j = 0; j < a.terms; j++) {
                    // check if a term with a certain variable already exist inside b, otherwise add it to the sum
                    if(sum.getMonomial(a.monomials[j].variable, a.monomials[j].power) == -1) {
                        sum.monomials[i] = a.monomials[j];
                    }
                }
            }
            return sum;
        }
};


int main() {
    Polynomial p1 = Polynomial();
    Polynomial p2 = Polynomial();
    cout << "\nPolynomial 1 insert: " << p1 << "\n";
    cout << "Polynomial 2 insert: " << p2 << "\n";
    Polynomial sum = p1+p2;
    cout << "Polynomial 1 + Polynomial 2 = " << sum << "\n";
    return 0;
}