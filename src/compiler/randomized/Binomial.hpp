#ifndef BINOMIAL_HPP_
#define BINOMIAL_HPP_

class Binomial {
public:
    Binomial(int);
    ~Binomial();
     unsigned long Choose(unsigned int, unsigned int);

private:
    bool Contains(unsigned int, unsigned int);

    int max;
    unsigned int **table;
};


#endif /* BINOMIAL_HPP_ */