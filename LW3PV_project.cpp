#include <windows.h>

#include <iostream>
#include <fstream> 
#include <string>
#include <iomanip>

const std::string FILE_SAVED_INFO = "File successfully saved.";
const std::string RECORD_DELETED_INFO = "Record successfully deleted.";
const std::string TOTAL_RECORD = "record";
const std::string TOTAL_MATCH = "match";
const std::string PROGRAM_EXIT_INFO = "Bye, Cutie. It\'s a pleasure, as always =)";
const std::string FILE_OPEN_ERROR = "File open error, Cutie. Something\'s wrong...";
const std::string INT_EXPECTED_ERROR = "I expected number.\nCutie, be more attentive! =)";
const std::string MENU_ITEM_EXPECTED_ERROR = "I expected one of five menu items, Cutie.";
const std::string NO_MATCH_ERROR = "No match found, Cutie =)";
const std::string EMPTY_FILE_ERROR = "File is empty, Cutie. No data to work with =)";
const unsigned char g_vert = 179;
const int g_charElementWidthMax = 255;


struct Book
{
    char bookCode[15] = "";
    char bookName[35] = "";
    int bookPageAmount = 0;
};

char selectMenu();
void addBook();
void viewBook();
void searchBook();
void deleteBook();
void setConsoleColor(int);
void enlightenCutie(std::string, std::string);
void printSimpleLine(std::string);
void printIntersectLine(std::string);
void printMatch(int match, std::string);
void printHeader(std::string);
void printSimpleTable(Book[], int);
void printComplexTable(Book[], int, char*);
Book* createStructure();
int getBooksAmount();

int main()
{
    char choice;

    do
    {
        system("cls");
        choice = selectMenu();

        if (choice == 'a' || choice == '1') {
            system("cls");
            addBook();
            system("pause");
        }

        else if (choice == 'w' || choice == '2') {
            system("cls");
            viewBook();
            system("pause");
        }

        else if (choice == 's' || choice == '3') {
            system("cls");
            searchBook();
            system("pause");
        }

        else if (choice == 'd' || choice == '4') {
            system("cls");
            deleteBook();
            system("pause");
        }

        else if (choice == 'x' || choice == '5' || choice == 'q') {
        }

        else {
            enlightenCutie("warn", MENU_ITEM_EXPECTED_ERROR);
            system("pause");
        }

    } while (choice != 'x' && choice != '5' && choice != 'q');

    system("cls");
    enlightenCutie("info", PROGRAM_EXIT_INFO);

    return 0;
}

void addBook()
{
    std::ofstream outfile("books.dat");
    if (!outfile) {
        enlightenCutie("warn", FILE_OPEN_ERROR);
        exit(0);
    }

    int amount;

    setConsoleColor(3);
    std::cout << "Enter the amount of books: ";
    setConsoleColor(15);
    std::cin >> amount;
    if (!std::cin) {
        enlightenCutie("warn", INT_EXPECTED_ERROR);
        exit(0);
    }
    std::cin.ignore(32767, '\n');

    Book* books = new Book[amount];

    for (int i = 0; i < amount; i++)
    {
        setConsoleColor(15);
        std::cout << "\n" << i + 1 << ")";
        setConsoleColor(3);
        std::cout << "\nEnter book code: ";
        setConsoleColor(15);
        std::cin.getline(books[i].bookCode, 15);
        setConsoleColor(3);
        std::cout << "Enter book name: ";
        setConsoleColor(15);
        std::cin.getline(books[i].bookName, 35);
        setConsoleColor(3);
        std::cout << "Enter the amount of pages: ";
        setConsoleColor(15);
        std::cin >> books[i].bookPageAmount;
        if (!std::cin) {
            enlightenCutie("warn", INT_EXPECTED_ERROR);
        }
        std::cin.ignore(32767, '\n');
        setConsoleColor(7);
    }

    outfile.write(reinterpret_cast<char*>(books), amount * sizeof(Book));
    outfile.close();

    enlightenCutie("info", FILE_SAVED_INFO);
}

void viewBook()
{
    Book* books = createStructure();
    int amount = getBooksAmount();

    printHeader("Full book list:");
    printSimpleTable(books, amount);
    printMatch(amount, TOTAL_RECORD);
}

void searchBook()
{
    Book* books = createStructure();
    int amount = getBooksAmount();

    char searchName[g_charElementWidthMax];
    std::cin.ignore(32767, '\n');
    setConsoleColor(3);
    std::cout << "Enter book name to search: ";
    setConsoleColor(15);
    std::cin.getline(searchName, g_charElementWidthMax);
    std::cout << std::endl;

    int matchCount = 0;
    for (int i = 0; i < amount; i++)
    {
        if (strstr(books[i].bookName, searchName)) {
            matchCount++;
        }
    }

    if (matchCount < 1) {
        enlightenCutie("warn", NO_MATCH_ERROR);
        exit(0);
    }

    printHeader("Search Results:");
    printComplexTable(books, amount, searchName);
    printMatch(matchCount, TOTAL_MATCH);
}

void deleteBook()
{
    Book* books = createStructure();
    int amount = getBooksAmount();

    char searchName[g_charElementWidthMax];
    std::cin.ignore(32767, '\n');
    setConsoleColor(3);
    std::cout << "Enter book name to delete: ";
    setConsoleColor(15);
    std::cin.getline(searchName, g_charElementWidthMax);
    std::cout << std::endl;

    int matchCount = 0;
    for (int i = 0; i < amount; i++)
    {
        if (strstr(books[i].bookName, searchName)) {
            matchCount++;
        }
    }

    if (matchCount < 1) {
        enlightenCutie("warn", NO_MATCH_ERROR);
        exit(0);
    }

    printHeader("Delete Results:");
    printComplexTable(books, amount, searchName);
    printMatch(matchCount, TOTAL_MATCH);

    int softDeletedRecordConfirm = NULL;
    setConsoleColor(3);
    std::cout << "Enter ID number to delete, 0 to cancel: ";
    setConsoleColor(15);
    std::cin >> softDeletedRecordConfirm;
    if (!std::cin) {
        enlightenCutie("warn", INT_EXPECTED_ERROR);
        exit(0);
    }
    std::cin.ignore(32767, '\n');

    if (softDeletedRecordConfirm != 0) {

        std::ofstream outfile("books.dat");
        if (!outfile) {
            enlightenCutie("warn", FILE_OPEN_ERROR);
            exit(0);
        }

        softDeletedRecordConfirm = softDeletedRecordConfirm - 1;
        for (int i = 0; i < amount; i++)
        {
            if (i != softDeletedRecordConfirm) {
                outfile.write((char*)(&books[i]), sizeof(Book));
            }

        }

        outfile.close();

        enlightenCutie("info", RECORD_DELETED_INFO);

    }
}

void setConsoleColor(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void enlightenCutie(std::string loglevel, std::string cutiepie)
{
    if (loglevel == "warn") {
        setConsoleColor(4);
    }
    else {
        setConsoleColor(11);
    }

    std::cout << std::endl << cutiepie << std::endl << std::endl;
    setConsoleColor(7);
}

void printSimpleLine(std::string p)
{
    unsigned char start, line, finish;
    const unsigned char topleft = 218, hor = 196, topright = 191, midleft = 195, midright = 180, botleft = 192, botright = 217;

    if (p == "topline") {
        start = topleft;
        line = hor;
        finish = topright;
    }
    else if (p == "midline") {
        start = midleft;
        line = hor;
        finish = midright;
    }
    else {
        start = botleft;
        line = hor;
        finish = botright;
    }

    std::cout << start;
    for (int i = 0; i < 78; i++) {
        std::cout << line;
    }
    std::cout << finish << std::endl;
}

void printIntersectLine(std::string p)
{
    unsigned char start, line, intersect, finish;
    const unsigned char g_vertobot = 193, g_vertfromtop = 194, hor = 196, horfromleft = 195, hortoright = 180, cross = 197;
    if (p == "topline") {
        start = horfromleft;
        line = hor;
        intersect = g_vertfromtop;
        finish = hortoright;
    }
    else if (p == "midline") {
        start = horfromleft;
        line = hor;
        intersect = cross;
        finish = hortoright;
    }
    else {
        start = horfromleft;
        line = hor;
        intersect = g_vertobot;
        finish = hortoright;
    }

    std::cout << start;
    for (int i = 0; i < 5; i++) {
        std::cout << line;
    }
    std::cout << intersect;
    for (int i = 0; i < 16; i++) {
        std::cout << line;
    }
    std::cout << intersect;
    for (int i = 0; i < 36; i++) {
        std::cout << line;
    }
    std::cout << intersect;
    for (int i = 0; i < 18; i++) {
        std::cout << line;
    }
    std::cout << finish << std::endl;
}

void printHeader(std::string s)
{
    setConsoleColor(7);
    printSimpleLine("topline");
    std::cout << g_vert << std::setw(32) << " ";
    setConsoleColor(9);
    std::cout << s;
    setConsoleColor(7);
    std::cout << std::setw(32) << g_vert << std::endl;
    printIntersectLine("topline");
    std::cout << g_vert;
    setConsoleColor(3);
    std::cout << std::setw(4) << "Nr.";
    setConsoleColor(7);
    std::cout << " " << g_vert;
    setConsoleColor(3);
    std::cout << std::setw(15) << "Code";
    setConsoleColor(7);
    std::cout << " " << g_vert;
    setConsoleColor(3);
    std::cout << std::setw(35) << "Name";
    setConsoleColor(7);
    std::cout << " " << g_vert;
    setConsoleColor(3);
    std::cout << std::setw(17) << "Page amount";
    setConsoleColor(7);
    std::cout << " " << g_vert << std::endl;
    printIntersectLine("midline");
}

void printSimpleTable(Book structure[], const int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << g_vert;
        setConsoleColor(15);
        std::cout << std::setw(4) << i + 1;
        setConsoleColor(7);
        std::cout << " " << g_vert;
        setConsoleColor(15);
        std::cout << std::setw(15) << structure[i].bookCode;
        setConsoleColor(7);
        std::cout << " " << g_vert;
        setConsoleColor(15);
        std::cout << std::setw(35) << structure[i].bookName;
        setConsoleColor(7);
        std::cout << " " << g_vert;
        setConsoleColor(15);
        std::cout << std::setw(17) << structure[i].bookPageAmount;
        setConsoleColor(7);
        std::cout << " " << g_vert << std::endl;
    }

    printIntersectLine("botline");
}

void printComplexTable(Book structure[], const int size, char* searchName)
{
    for (int i = 0; i < size; i++)
    {
        if (strstr(structure[i].bookName, searchName)) {
            std::cout << g_vert;
            setConsoleColor(15);
            std::cout << std::setw(4) << i + 1;
            setConsoleColor(7);
            std::cout << " " << g_vert;
            setConsoleColor(15);
            std::cout << std::setw(15) << structure[i].bookCode;
            setConsoleColor(7);
            std::cout << " " << g_vert;
            setConsoleColor(15);
            std::cout << std::setw(35) << structure[i].bookName;
            setConsoleColor(7);
            std::cout << " " << g_vert;
            setConsoleColor(15);
            std::cout << std::setw(17) << structure[i].bookPageAmount;
            setConsoleColor(7);
            std::cout << " " << g_vert << std::endl;
        }
    }

    printIntersectLine("botline");
}

char selectMenu()
{
    char v;

    setConsoleColor(9);
    std::cout << "Please choose an option:\n\n";
    setConsoleColor(15);
    std::cout << "1";
    setConsoleColor(9);
    std::cout << ". (";
    setConsoleColor(15);
    std::cout << "A";
    setConsoleColor(9);
    std::cout << ")dd books.\n";
    setConsoleColor(15);
    std::cout << "2";
    setConsoleColor(9);
    std::cout << ". Vie(";
    setConsoleColor(15);
    std::cout << "w";
    setConsoleColor(9);
    std::cout << ") books.\n";
    setConsoleColor(15);
    std::cout << "3";
    setConsoleColor(9);
    std::cout << ". (";
    setConsoleColor(15);
    std::cout << "S";
    setConsoleColor(9);
    std::cout << ")earch books.\n";
    setConsoleColor(15);
    std::cout << "4";
    setConsoleColor(9);
    std::cout << ". (";
    setConsoleColor(15);
    std::cout << "D";
    setConsoleColor(9);
    std::cout << ")elete books.\n";
    setConsoleColor(15);
    std::cout << "5";
    setConsoleColor(9);
    std::cout << ". E(";
    setConsoleColor(15);
    std::cout << "x";
    setConsoleColor(9);
    std::cout << ")it program.\n";
    std::cout << "\nYour option? ";

    setConsoleColor(15);
    std::cin >> v;
    setConsoleColor(7);

    if (isupper(v)) {
        v = tolower(v);
    }

    return v;
}

Book* createStructure()
{
    int amount = getBooksAmount();
    Book* books = new Book[amount];

    std::ifstream infile("books.dat");
    if (!infile) {
        enlightenCutie("warn", FILE_OPEN_ERROR);
        exit(0);
    }

    infile.seekg(0);
    infile.read(reinterpret_cast<char*>(books), amount * sizeof(Book));
    infile.close();

    if (amount < 1) {
        enlightenCutie("warn", EMPTY_FILE_ERROR);
        exit(0);
    }

    return books;
}

int getBooksAmount() {
    std::ifstream infile("books.dat");
    if (!infile) {
        enlightenCutie("warn", FILE_OPEN_ERROR);
        exit(0);
    }

    infile.seekg(0, std::ios::end);
    int fsize = infile.tellg();
    int amount = fsize / sizeof(Book);
    infile.close();

    return amount;
}

void printMatch(int match, std::string type)
{
    int offset = 0;
    std::string missingLetter = "";

    if (type == TOTAL_RECORD) {
        offset = 62;
    }
    else {
        offset = 63;
        missingLetter = "e";
    }

    std::cout << g_vert;
    setConsoleColor(3);

    if (match == 1) {
        std::cout << std::setw(offset) << "Total ";
        setConsoleColor(15);
        std::cout << match;
        setConsoleColor(3);
        std::cout << " " << type << " found.";
    }

    else {
        std::cout << std::setw(61) << "Total ";
        setConsoleColor(15);
        std::cout << match;
        setConsoleColor(3);
        std::cout << " " << type << missingLetter << "s found.";
    }

    setConsoleColor(7);
    std::cout << " " << g_vert << std::endl;
    printSimpleLine("botline");
}