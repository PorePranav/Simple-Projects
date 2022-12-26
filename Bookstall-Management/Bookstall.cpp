#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
class FileOpeningException : public exception{
    public:
        FileOpeningException() = default;
        ~FileOpeningException() = default;
        const char* what() const noexcept{
            return "Could not open the file";
        }
};
class I_Book{
    public:
        virtual string getName() const = 0;
        virtual string getAuthor() const = 0;
        virtual string getPublisher() const = 0;
        virtual string getPrintType() const = 0;
        virtual string getIsbn() const = 0;
        virtual int getQuantity() const = 0;
        virtual double getPrice() const = 0;
        virtual void setQuantity(int) = 0;
};
class Book : public I_Book{
    private:
        string name, author, publisher, printType, isbn;
        friend class Inventory;
        friend class Cart;
    protected:
        int quantity;
        double price;
        friend ostream& operator<<(ostream&, const Book&);
    public:
        Book() = default;
        Book(string name, string author, string publisher, string printType, string isbn, int quantity, double price) : 
            name(name), author(author), publisher(publisher), printType(printType), isbn(isbn), quantity(quantity), price(price) {}
        Book(string *name, string *author, string *publisher, string *printType, string *isbn, int quantity, double* price){
            this->name = *name;
            this->author = *author;
            this->publisher = *publisher;
            this->printType = *printType;
            this->isbn = *isbn;
            this->quantity = quantity;
            this->price = *price;
        }
        string getName() const override {return name;}
        string getAuthor() const override {return author;}
        string getPublisher() const override {return publisher;}
        string getPrintType() const override {return printType;}
        string getIsbn() const override {return isbn;}
        int getQuantity() const override {return quantity;}
        double getPrice() const override {return price;}
        void setQuantity(int newQuantity) override {quantity = newQuantity;}
};
ostream& operator<<(ostream &os, const Book &obj){
    os<<obj.name<<" "<<obj.author<<" "<<obj.publisher<<" "<<obj.printType<<" "<<obj.isbn<<" "<<obj.quantity<<" "<<obj.price;
    return os;
}
class EnglishBook : virtual public Book{
    private:
        string language;
    public: 
        EnglishBook(string name, string author, string publisher, string printType, string isbn, int quantity, double price) : 
            Book(name, author, publisher, printType, isbn, quantity, price) {language = "English";}
        EnglishBook() = default;
        virtual void printBookType(){cout<<"English Book"<<endl;}
         
};
class Children : public EnglishBook{
    public:
        Children() = default;
        virtual void printBookType(){cout<<"Children's Book"<<endl;}
};
class Fiction : public EnglishBook{
    public:
        Fiction() = default;
        virtual void printBookType(){cout<<"Fiction Book"<<endl;}
};
class NonFiction : public EnglishBook{
    public:
        NonFiction() = default;
        virtual void printBookType(){cout<<"Non-Fiction Book"<<endl;}
};
class Autobiography : public EnglishBook{
    public:
        Autobiography() = default;
        virtual void printBookType(){cout<<"Autobiography"<<endl;}
};
class Inventory{
    private:
        vector<Book> books;
        double totalPrice = 0;
        friend class Cart;
    public:
        Inventory() = default;
        double getTotalPrice() const {return totalPrice;}
        vector<Book> getBooks() const {return books;}
        void addBook();
        void updateBook();
        void searchBook();
        vector<Book> searchByName();
        Book& searchByName(string&);
        vector<Book> searchByAuthor();
        vector<Book> searchByPublisher();
        vector<Book> searchByIsbn();
        void printInventory();
        void printInventory(const vector<Book>&);
        int getIndex(const Book&);
        void deleteBook();
        template<typename T>
        void readFromDatabase();
        template<typename T>
        void writeToDatabase();
};
void Inventory::addBook(){
    string name, author, publication, isbn, printingType;
    int quantity;
    double price;

    while(true){
        cout<<"Enter the name of the book: (Enter -1 to stop adding books) ";
        getline(cin, name);

        if(name == "-1"){
            break;
            cout<<endl;
        }

        cout<<"Enter the author: ";
        getline(cin, author);
        
        cout<<"Enter the publication: ";
        getline(cin, publication);
        
        cout<<"Enter the ISBN code: ";
        getline(cin, isbn);
        
        cout<<"Enter the type of printing: ";
        getline(cin, printingType);
        
        cout<<"Enter the quantity: ";
        cin>>quantity;
        
        cout<<"Enter the price: ";
        cin>>price;

        cin.ignore();
        Book newBook(name, author, publication, printingType, isbn, quantity, price);
        books.push_back(newBook);

        totalPrice += quantity * price;

        cout<<endl;
    }
}
void Inventory::updateBook(){
    string targetName;
    cout<<"Enter the name of the book you want to update: ";
    getline(cin, targetName);

    Book& targetBook = searchByName(targetName);
    if(targetBook.getName() == "")
        cout<<"No such book found"<<endl;
    else{
        cout<<"Found record: ";
        cout<<targetBook<<endl;
        int choice;

        cout<<"1. Update the name of the book"<<endl;
        cout<<"2. Update the author name"<<endl;
        cout<<"3. Update the publication"<<endl;
        cout<<"4. Update the type of printing"<<endl;
        cout<<"5. Update the ISBN number"<<endl;
        cout<<"6. Update the quantity in stock"<<endl;
        cout<<"7. Update the price"<<endl;
        cin>>choice;
        cin.ignore();
        switch(choice){
            case 1:{
                string updatedName;
                cout<<"Enter the updated name: ";
                getline(cin, updatedName);
                targetBook.name = updatedName;
                break;
            }
            case 2:{
                string updatedAuthor;
                cout<<"Enter the updated author: ";
                getline(cin, updatedAuthor);
                targetBook.author = updatedAuthor;
                break;
            }
            case 3:{
                string updatedPublisher;
                cout<<"Enter the updated publisher: ";
                getline(cin, updatedPublisher);
                targetBook.publisher = updatedPublisher;
                break;
            }
            case 4:{
                string updatedPrinting;
                cout<<"Enter the updated printing type: ";
                getline(cin, updatedPrinting);
                targetBook.printType = updatedPrinting;
                break;
            }
            case 5:{
                string updatedISBN;
                cout<<"Enter the updated ISBN: ";
                getline(cin, updatedISBN);
                targetBook.isbn = updatedISBN;
                break;
            }
            case 6:{
                int updatedQuantity;
                cout<<"Enter the updated quantity: ";
                cin>>updatedQuantity;
                targetBook.quantity = updatedQuantity;
                break;
            }
            case 7:{
                int updatedPrice;
                cout<<"Enter the updated price: ";
                cin>>updatedPrice;
                targetBook.price = updatedPrice;
                break;
            }
        }
        cout<<"After updating: "<<targetBook<<endl;
    }
}
void Inventory::searchBook(){
    int choice;
    cout<<"1. Search a book by name"<<endl;
    cout<<"2. Search a book by author"<<endl;
    cout<<"3. Search a book by publication"<<endl;
    cout<<"4. Search a book by ISBN code"<<endl;
    cin>>choice;

    switch(choice){
        case 1:{
            vector<Book> byName = searchByName();
            if(byName.size() == 0)
                cout<<"No books found "<<endl;
            else
                printInventory(byName);
            break;
        }
            
        case 2:{ 
            vector<Book> byAuthor = searchByAuthor();
            if(byAuthor.size() == 0)
                cout<<"No books found "<<endl;
            else
                printInventory(byAuthor);
            break;
        }

        case 3:{
            vector<Book> byPublication = searchByPublisher();
            if(byPublication.size() == 0)
                cout<<"No books found "<<endl;
            else
                printInventory(byPublication);
            break;
        }

        case 4:{
            vector<Book> byIsbn = searchByIsbn();
            if(byIsbn.size() == 0)
                cout<<"No books found "<<endl;
            else
                printInventory(byIsbn);
            break;
        }

        default:
            cout<<"Enter a valid choice"<<endl;
    }
}
vector<Book> Inventory::searchByName(){
    vector<Book> result;
    string searchName;
    cout<<"Enter the name of the book you want to search: ";
    cin.ignore();
    getline(cin, searchName);
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    for(size_t i = 0; i < books.size(); i++){
        string targetName = books.at(i).getName();
        transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);
        if(searchName == targetName){
            result.push_back(books.at(i));
            break;
        }
    }
    return result;
}
Book& Inventory::searchByName(string &targetName){
    transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);
    for(size_t i=0; i<books.size(); i++){
        string searchName = books.at(i).getName();
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        if(targetName == searchName){
            return books.at(i);
        }
    }
    Book *newBook = new Book;
    return *newBook;
}
vector<Book> Inventory::searchByAuthor(){
    vector<Book> result;
    string searchName;
    cout<<"Enter the name of the author to search: ";
    cin.ignore();
    getline(cin, searchName);
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    for(size_t i = 0; i < books.size(); i++){
        string targetName = books.at(i).getAuthor();
        transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);
        if(searchName == targetName)
            result.push_back(books.at(i));
    }
    return result;
}
vector<Book> Inventory::searchByPublisher(){
    vector<Book> result;
    string searchName;
    cout<<"Enter the name of the publisher to search: ";
    cin.ignore();
    getline(cin, searchName);
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    for(size_t i = 0; i < books.size(); i++){
        string targetName = books.at(i).getPublisher();
        transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);
        if(searchName == targetName)
            result.push_back(books.at(i));
    }
    return result;
}
vector<Book> Inventory::searchByIsbn(){
    vector<Book> result;
    string searchName;
    cout<<"Enter the ISBN code of the book you want to search: ";
    cin.ignore();
    getline(cin, searchName);
    transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    for(size_t i = 0; i < books.size(); i++){
        string targetName = books.at(i).getIsbn();
        transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);
        if(searchName == targetName){
            result.push_back(books.at(i));
            break;
        }
    }
    return result;
}
void Inventory::printInventory(){
    if(books.size() == 0){
        cout<<"Inventory is empty"<<endl;
        cout<<endl;
    }
    else{
        cout<<"\n======================Printing Inventory ========================="<<endl;
        for_each(books.begin(), books.end(), [](Book obj){cout<<obj<<endl;});
        cout<<endl;
    }
}
void Inventory::printInventory(const vector<Book> &searchBooks){
    for_each(searchBooks.begin(), searchBooks.end(), [](Book obj){cout<<obj<<endl;});
    cout<<endl;
}
int Inventory::getIndex(const Book &target){
    for(size_t i = 0; i < books.size(); i++){
        if(books.at(i).name == target.name)
            return i;
    }
    return -1;
}
void Inventory::deleteBook(){
    string targetBookName;
    cout<<"Enter the name of the book you want to delete: ";
    getline(cin, targetBookName);

    Book& targetBook = searchByName(targetBookName);
    if(targetBook.getName() == "") 
        cout<<"No such book found"<<endl;
    else{
        cout<<"Found record: "<<targetBook<<endl;
        char confirmation;
        cout<<"\nConfirm that you want to delete the book: (y/n)";
        cin>>confirmation;
        if(confirmation == 'Y' || confirmation == 'y'){
            books.erase(books.begin() + getIndex(targetBook));
            cout<<"Book removed successfully"<<endl;
        }
    }
}
template<typename T>
void Inventory::readFromDatabase(){
    T in_file {"data.txt"}; 
    try{
        if(!in_file.is_open())
            throw FileOpeningException();
        else{
            string numberOfBooksString;
            getline(in_file, numberOfBooksString);
            int numberOfBooks = stoi(numberOfBooksString);
            for(int i = 0; i < numberOfBooks; i++){
                string name, author, publisher, printType, isbn, quantityString, priceString;
                int quantity;
                double price;
                getline(in_file, name);
                getline(in_file, author);
                getline(in_file, publisher);
                getline(in_file, isbn);
                getline(in_file, printType);
                getline(in_file, quantityString);
                getline(in_file, priceString);
                quantity = stoi(quantityString);
                price = stod(priceString);
                Book newBook(name, author, publisher, printType, isbn, quantity, price);
                books.push_back(newBook);
            }
            if(numberOfBooks == 0)
                cout<<"No data in the database"<<endl;
            in_file.close();
        }
    }
    catch(const FileOpeningException &e){
        cerr<<e.what()<<endl;
    }
}
template<typename T>
void Inventory::writeToDatabase(){
    T out_file{"data.txt"};
    try{
        if(!out_file.is_open())
            throw FileOpeningException();
        else{
            out_file<<books.size()<<endl;
            for(size_t i = 0; i < books.size(); i++){
                out_file<<books.at(i).getName()<<endl;
                out_file<<books.at(i).getAuthor()<<endl;
                out_file<<books.at(i).getPublisher()<<endl;
                out_file<<books.at(i).getIsbn()<<endl;
                out_file<<books.at(i).getPrintType()<<endl;
                out_file<<books.at(i).getQuantity()<<endl;
                out_file<<books.at(i).getPrice()<<endl;
            }   
            out_file.close();
        }    
    }
    catch(const FileOpeningException &e){
        cerr<<e.what()<<endl;
    }  
}
class Cart{
    private:
        vector<Book> cartItems;
        double totalCost;
        friend class Customer;
    public:
        Cart(){totalCost = 0;}
        vector<Book>& getCartItems(){return cartItems;}
        int getTotalCost(){return totalCost;}
        void addToCart(Inventory&);
        void deleteFromCart(Inventory&);
        void eraseCart(Inventory&);
        void printCart();
        Book& searchByName(string&);
        int getIndex(const Book&);
};
void Cart::addToCart(Inventory& inventory){
    inventory.printInventory();
    string bookName;
    cout<<"Enter the name of the book you wish to purchase: ";
    cin.ignore();
    getline(cin, bookName);
    Book targetBook = inventory.searchByName(bookName);
    if(targetBook.getName() == "")
        cout<<"No such book found"<<endl;
    else{
        cout<<"Found record: "<<targetBook<<endl;
        char confirmation;
        cout<<"\nConfirm that you want to purchase the book: (y/n) ";
        cin>>confirmation;
        if(confirmation == 'Y' || confirmation == 'y'){
            int quantityToPurchase = 0;
            cout<<"Enter the quantity you wish to purchase: ";
            cin>>quantityToPurchase;
            if(targetBook.getQuantity() < quantityToPurchase)
                cout<<"Only "<<targetBook.getQuantity()<<" quantity is available in stock"<<endl;
            else{
                string *name, *author, *publisher, *printType, *isbn;
                double *price;
                name = new string;
                author = new string;
                publisher = new string;
                printType = new string;
                isbn = new string;
                price = new double;
                *name = targetBook.getName();
                *author = targetBook.getAuthor();
                *publisher = targetBook.getPublisher();
                *printType = targetBook.getPrintType();
                *isbn = targetBook.getIsbn();
                *price = targetBook.getPrice();
                Book *toCart = new Book(name, author, publisher, printType, isbn, quantityToPurchase, price);

                cartItems.push_back(*toCart);
                int bookIndex = inventory.getIndex(targetBook);
                int newQuantity = targetBook.getQuantity() - quantityToPurchase;
                inventory.books.at(bookIndex).setQuantity(newQuantity);
                totalCost += quantityToPurchase * *(price);
                cout<<"Successfully added book to the cart"<<endl;
                delete name;
                delete author;
                delete publisher;
                delete printType;;
                delete isbn;
                delete price;
                delete toCart;
            }
        }
    }
    cin.ignore();
}
void Cart::deleteFromCart(Inventory& inventory){
    printCart();
    string bookName;
    cout<<"Enter the name of the book you wish to delete from the cart: ";
    cin.ignore();
    getline(cin, bookName);
    Book targetBook = searchByName(bookName);
    if(targetBook.getName() == "")
        cout<<"No such book found"<<endl;
    else{
        cout<<"Found record: "<<targetBook<<endl;
        char confirmation;
        cout<<"\nConfirm that you want to delete the book from the cart: (y/n) ";
        cin>>confirmation;
        if(confirmation == 'Y' || confirmation == 'y'){
            totalCost -= targetBook.getQuantity() * targetBook.getPrice();
            int inventoryBookIndex = inventory.getIndex(targetBook);

            int cartBookIndex = getIndex(targetBook);
            int originalQuantity = inventory.books.at(inventoryBookIndex).getQuantity();
            int cartBookQuantity = cartItems.at(cartBookIndex).getQuantity();
            int newQuantity = originalQuantity + cartBookQuantity;

            inventory.books.at(inventoryBookIndex).setQuantity(newQuantity);
            cartItems.erase(cartItems.begin() + cartBookIndex);
        }
    }
    
}
void Cart::eraseCart(Inventory &inventory){
    char confirmation;
    cout<<"Confirm that you want to erase the cart (y/n): ";
    cin>>confirmation;
    if(confirmation == 'y' || confirmation=='Y'){
        while(cartItems.size() != 0){
            totalCost -= cartItems.at(0).getQuantity() * cartItems.at(0).getPrice();
            int inventoryBookIndex = inventory.getIndex(cartItems.at(0));
            int originalQuantity = inventory.books.at(inventoryBookIndex).getQuantity();
            int newQuantity = originalQuantity + cartItems.at(0).getQuantity();
            inventory.books.at(inventoryBookIndex).setQuantity(newQuantity);
            cartItems.erase(cartItems.begin());
        }
    }
}
void Cart::printCart(){
    cout<<"========================Printing Cart Items====================="<<endl;
    for_each(cartItems.begin(), cartItems.end(), [](Book obj){cout<<obj<<endl;});
    cout<<"\nTotal Cost: "<<totalCost<<endl;
}
Book& Cart::searchByName(string& targetName){
    transform(targetName.begin(), targetName.end(), targetName.begin(), ::tolower);
    for(size_t i=0; i<cartItems.size(); i++){
        string searchName = cartItems.at(i).getName();
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
        if(targetName == searchName){
            return cartItems.at(i);
        }
    }
    Book *newBook = new Book;
    return *newBook;
}
int Cart::getIndex(const Book &target){
    for(size_t i = 0; i < cartItems.size(); i++){
        if(cartItems.at(i).name == target.name)
            return i;
    }
    return -1;
}
class Customer{
    private:
        string name, userId;
        Cart* cart;
    public:
        Customer() = default;
        Customer(string name, string userId){
            this->name = name;
            this->userId = userId;
            cart = new Cart();
        }
        string getName(){return name;}
        string getUserId(){return userId;}
        void printAvailableStock(Inventory&);
        Cart& getCart(){return *cart;}
        void generateSalesBill();
};
void Customer::printAvailableStock(Inventory& inventory){
    cout<<"Book \t\tAuthor\t\tPrice Quantity"<<endl;
    for(size_t i = 0; i < inventory.getBooks().size(); i++){
        string name = inventory.getBooks().at(i).getName();
        string author = inventory.getBooks().at(i).getAuthor();
        int quantity = inventory.getBooks().at(i).getQuantity();
        int rate = inventory.getBooks().at(i).getPrice();
        cout<<name<<'\t'<<author<<'\t'<<quantity<<'\t'<<rate<<endl;
    }
}
void Customer::generateSalesBill(){
    cout<<"============================Sales Bill========================"<<endl;
    cout<<"Book\t\tQuantity  Rate\tTotal"<<endl;
    while(this->getCart().cartItems.size() != 0){
        string name = this->getCart().getCartItems().at(0).getName();
        int quantity = this->getCart().getCartItems().at(0).getQuantity();
        int rate = this->getCart().getCartItems().at(0).getPrice();
        int totalForBook = quantity * rate;
        cout<<name<<'\t'<<quantity<<'\t'<<" "<<rate<<'\t'<<totalForBook<<endl;
        this->getCart().cartItems.erase(this->getCart().cartItems.begin());
    }
    cout<<"Total Bill: "<<this->getCart().totalCost<<endl;
    this->getCart().totalCost = 0;    
}
void printMenu();
void transferToCustomer(Inventory&);
void printCustomerMenu();
int main(){
    Inventory inventory;
    inventory.readFromDatabase<ifstream>();
    int choice;
    if(inventory.getBooks().size() == 0){
        char confirmation;
        cout<<"There are no books available in the inventory, would you like to add some? (y/n): ";
        cin>>confirmation;
        cin.ignore();
        if(confirmation == 'y' || confirmation == 'Y')
            inventory.addBook();
    }
    while(true){
        printMenu();
        cin>>choice;
        cin.ignore();
        cout<<endl;
        switch(choice){
            case 1:{
                inventory.addBook();
                break;
            }
            case 2:{
                inventory.updateBook();
                break;
            }
            case 3:{
                inventory.searchBook();
                break;
            }
            case 4:{
                inventory.deleteBook();
                break;
            }
            case 5:{
                inventory.printInventory();
                break;
            }
            case 6:{
                transferToCustomer(inventory);
                break;
            }
            case 7:{
                inventory.writeToDatabase<ofstream>();
                cout<<"Exiting the application....."<<endl;
                return 0;
            }
            default:{
                cout<<"Please enter a correct choice"<<endl;
                break;
            }
        }
    }
    return 0;
}
void printMenu(){
    cout<<endl;
    cout<<"1. Add books to the inventory."<<endl;
    cout<<"2. Update records for a book."<<endl;
    cout<<"3. Search for a book in the inventory."<<endl;
    cout<<"4. Delete a book from the inventory."<<endl;
    cout<<"5. Print the available inventory."<<endl;
    cout<<"6. Transfer to customer control."<<endl;
    cout<<"7. Exit the program."<<endl;

    cout<<"Enter your choice: ";
}
void transferToCustomer(Inventory& inventory){
    string name, user_id;
    cout<<"Enter your name: ";
    cin>>name;
    user_id = "2001";
    Customer newCustomer(name, user_id);
    int customerChoice;
    while(true){
        printCustomerMenu();
        cin>>customerChoice;

        switch(customerChoice){
            case 1:{
                newCustomer.printAvailableStock(inventory);
                break;
            }
            case 2:{
                inventory.searchBook();
                break;
            } 
            case 3:{
                newCustomer.getCart().addToCart(inventory);
                break;
            }
            case 4:{
                newCustomer.getCart().printCart();
                break;
            }
            case 5:{
                newCustomer.getCart().deleteFromCart(inventory);
                break;
            }
            case 6:{
                newCustomer.getCart().eraseCart(inventory);
                break;
            }
            case 7:{
                newCustomer.generateSalesBill();
                break;
            }
            case 8:{
                goto exit;
            }
            default:
                cout<<"Please enter a correct choice."<<endl;
                break;
        }
    }
    exit:{}
}
void printCustomerMenu(){
    cout<<endl;
    cout<<"1. Print available books."<<endl;
    cout<<"2. Search for a book in the inventory."<<endl;
    cout<<"3. Add a book to the cart."<<endl;
    cout<<"4. Print the items in the cart."<<endl;
    cout<<"5. Delete a book from the cart."<<endl;
    cout<<"6. Delete every book from the cart."<<endl;
    cout<<"7. To generate your sales bill."<<endl;
    cout<<"8. To go back to admin control."<<endl;
    cout<<"Enter your choice: ";
}
