#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


const size_t sep = 10;

struct Person {
    std::string name;
    size_t age;
    size_t swag;
    bool clever;

    Person(std::string n, size_t a, size_t s, bool c) : name(n), age(a),
                                                        swag(s), clever(c) {}
};

std::ostream &operator<<(std::ostream &out, const Person &p) {
    std::string is_clever = p.clever ? "Yes" : "No";

    out << std::setw(sep) << p.name
        << std::setw(sep) << p.age
        << std::setw(sep) << p.swag
        << std::setw(sep) << is_clever
        << std::endl;

    return out;
}

void print(const std::vector <Person> &people, const std::string sort_type) {
    std::cout << std::setw(2 * sep) << sort_type
              << std::setw(sep) << 0
              << people[0]
              << std::setw(2 * sep) << ""
              << std::setw(sep) << 1
              << people[1] << std::endl;
}


int main() {
    Person p1("Felix", 12, 99, true);
    Person p2("Anian", 12, 100, false);

    std::vector <Person> people = {p1, p2};

    std::cout << std::setw(2 * sep) << "SORTED BY (ASCENDING)"
              << std::setw(sep) << "RANK"
              << std::setw(sep) << "NAME"
              << std::setw(sep) << "AGE"
              << std::setw(sep) << "SWAG"
              << std::setw(sep) << "CLEVER"
              << std::endl;

    // Sort by Name (Ascending)
    std::sort(people.begin(), people.end(), [](Person a, Person b) {
        return a.name < b.name;
    });
    print(people, "Name");

    // Sort by Swag (Ascending)
    std::sort(people.begin(), people.end(), [](Person a, Person b) {
        return a.swag < b.swag;
    });
    print(people, "Swag");

    // Lexicographically Sort by Age and Cleverness (Ascending)
    std::sort(people.begin(), people.end(), [](Person a, Person b) {
        return a.age < b.age || (a.age == b.age && a.clever < b.clever);
    });
    print(people, "Age and Cleverness");

    return 0;
}
