#ifndef BINOMIAL_HPP_
#define BINOMIAL_HPP_

class Binomial {
public:
    Binomial(int);
    ~Binomial();
     unsigned long long Choose(unsigned int, unsigned int);

private:
    bool Contains(unsigned long long, unsigned long long);

    int max;
    unsigned long long **table;
};


#endif /* BINOMIAL_HPP_ */