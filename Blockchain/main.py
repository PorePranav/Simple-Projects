from blockchain import Blockchain, Block
from data import Student, Slot, Faculty

if __name__ == "__main__":
    student_blockchain = Blockchain()
    slots_blockchain = Blockchain()
    faculty_blockchain = Blockchain()

    Student.read_from_file(student_blockchain)
    Faculty.read_from_file(faculty_blockchain)
    Slot.read_from_file(slots_blockchain, student_blockchain)

    def print_data():
        print("1. Print student blockchain")
        print("2. Print faculty blockchain")
        print("3. Print slots blockchain")

        print_choice = int(input("Enter your choice\t"))

        if print_choice == 1:
            student_blockchain.print_blockchain()
        elif print_choice == 2:
            faculty_blockchain.print_blockchain()
        elif print_choice == 3:
            slots_blockchain.print_blockchain()

    def search_data():
        print("1. Search student details")
        print("2. Search faculty details")
        print("3. Search slot details")

        search_choice = int(input("Enter your choice\t"))

        if search_choice == 1:
            Student.search_students(student_blockchain)
        elif search_choice == 2:
            Faculty.search_faculty(faculty_blockchain)
        elif search_choice == 3:
            Slot.search_slots(slots_blockchain)

    def search_student_marks():
        input_registration_number = input("Enter the registration number of the student:\t")
        for block in student_blockchain.chain[:2:-1]:
            if block.data.registration_number == input_registration_number:
                print(block.data.marks)
                return
        print(f"No student with registration number {input_registration_number} found")

    def edit_student_marks():
        input_registration_number = input("Enter the registration number of the student:\t")
        for block in student_blockchain.chain[:2:-1]:
            student_block = block.data
            if student_block.registration_number == input_registration_number:
                target_key = input("Enter the course code\t")
                new_marks = input("Enter the new marks\t")
                new_marks_dictionary = block.data.marks.copy()
                new_marks_dictionary[target_key] = new_marks
                updated_student = Student(student_block.registration_number, student_block.first_name,
                                          student_block.last_name, student_block.email, student_block.gender,
                                          student_block.registered_courses, new_marks_dictionary,
                                          student_block.attendance)
                new_block = Block(student_blockchain.last_block.index,
                                  updated_student, student_blockchain.last_block.hash)
                student_blockchain.add_to_chain(new_block)
                print("Updated marks: ")
                print(new_marks_dictionary)
                return
        print(f"No student with registration number {input_registration_number} found")

    def search_student_attendance():
        input_registration_number = input("Enter the registration number of the student\t")
        for block in student_blockchain.chain[:2:-1]:
            if block.data.registration_number == input_registration_number:
                print(block.data.attendance)
                return
        print(f"No student with {input_registration_number} found")

    def edit_student_attendance():
        input_registration_number = input("Enter the registration number of the student:\t")
        for block in student_blockchain.chain[:2:-1]:
            student_block = block.data
            if student_block.registration_number == input_registration_number:
                target_key = input("Enter the course code\t")
                new_attendance = input("Enter the new attendance percentage\t")
                new_attendance_dictionary = block.data.attendance.copy()
                new_attendance_dictionary[target_key] = new_attendance
                updated_student = Student(student_block.registration_number, student_block.first_name,
                                          student_block.last_name, student_block.email, student_block.gender,
                                          student_block.registered_courses, student_block.marks,
                                          new_attendance_dictionary)
                new_block = Block(student_blockchain.last_block.index,
                                  updated_student, student_blockchain.last_block.hash)
                student_blockchain.add_to_chain(new_block)
                print("Updated attendance is: ")
                print(new_attendance_dictionary)
                return
        print(f"No student with registration number {input_registration_number} found")

    def print_menu():
        print("1. Print data")
        print("2. Search for data")
        print("3. Lookup student marks")
        print("4. Edit student marks")
        print("5. Lookup student attendance")
        print("6. Edit student attendance")
        print("7. To exit the application\n")

    while True:
        print_menu()
        choice = int(input("Enter your choice\t"))

        if choice == 1:
            print_data()

        elif choice == 2:
            search_data()

        elif choice == 3:
            search_student_marks()

        elif choice == 4:
            edit_student_marks()

        elif choice == 5:
            search_student_attendance()

        elif choice == 6:
            edit_student_attendance()

        elif choice == 7:
            Student.write_to_file(student_blockchain)
            break
