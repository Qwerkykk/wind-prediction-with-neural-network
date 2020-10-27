#ifndef  DATAVECTOR_H
#define  DATAVECTOR_H

#include <string>
#include <vector>

class DataVector {
private:
    std::string id;
    std::vector<double> *components;
    int dimension;
public:
    DataVector();

    DataVector(std::string id, std::vector<double> &components);

    ~DataVector();

    void setId(const std::string &id);

    void setComponents( std::vector<double> &components);

    void setDimension(int dim);

    void addComponent(double component);

    const std::vector<double> &getComponents() const;

    int getDimension();

    const std::string &getId() const;

    void pad(int maxLength, double value);

    DataVector& myContents();

    void clear();
};


#endif // DATAVECTOR_H
