from blockchain import Block


class Student:
    def __init__(self, registration_number, first_name, last_name, email, gender, registered_courses, marks,
                 attendance):
        self.registration_number = registration_number
        self.first_name = first_name
        self.last_name = last_name
        self.full_name = first_name + " " + last_name
        self.email = email
        self.gender = gender
        self.registered_courses = registered_courses
        self.marks = marks
        self.attendance = attendance

    def to_string(self):
        return f"[Registration Number: {self.registration_number},\nFull Name: {self.full_name},\nEmail: {self.email},"\
               f"\nGender: {self.gender}] "

    def serialize(self):
        return f"{self.registration_number},{self.first_name},{self.last_name},{self.email},{self.gender}," \
               f"{self.marks['ECE2002']},{self.marks['MAT2001']},{self.marks['CSE2003']},{self.marks['MGT1002']}," \
               f"{self.marks['MEE2014']},{self.marks['CSE2006']},{self.attendance['ECE2002']}," \
               f"{self.attendance['MAT2001']},{self.attendance['CSE2003']},{self.attendance['MGT1002']}," \
               f"{self.attendance['MEE2014']},{self.attendance['CSE2006']}"

    @staticmethod
    def write_to_file(blockchain):
        write_file = open("new_students_backup.csv", "w")
        data = ""
        for block in blockchain.chain[1:]:
            data += block.data.serialize() + "\n"

        write_file.write(data)
        write_file.close()

    @staticmethod
    def read_from_file(blockchain):
        read_file = open("new_students_backup.csv", "r")
        for line in read_file:
            data = line.rstrip('\n').split(',')

            registration_number = data[0]
            data.pop(0)

            first_name = data[0]
            data.pop(0)

            last_name = data[0]
            data.pop(0)

            email = data[0]
            data.pop(0)

            gender = data[0]
            data.pop(0)

            marks = {"ECE2002": data[0], "MAT2001": data[1], "CSE2003": data[2], "MGT1002": data[3],
                     "MEE2014": data[4], "CSE2006": data[5]}

            attendance = {"ECE2002": data[6], "MAT2001": data[7], "CSE2003": data[8], "MGT1002": data[9],
                          "MEE2014": data[10], "CSE2006": data[11]}

            new_student = Student(registration_number, first_name, last_name, email, gender, [], marks, attendance)
            new_student_block = Block(blockchain.chain[-1].index + 1, new_student, blockchain.chain[-1].hash)
            blockchain.add_to_chain(new_student_block)

    @staticmethod
    def search_students(blockchain):
        print("1. Search student by registration number")
        print("2. Search student by name")

        choice = int(input("Enter your choice "))

        if choice == 1:
            reg_no = input("Enter the registration number of the student ")
            for student in blockchain.chain[1:]:
                if student.data.registration_number == reg_no:
                    print(student.to_string())
                    return
            else:
                print("No student found with registration number: " + reg_no + '\n')

        elif choice == 2:
            name = input("Enter name of the student ")
            for student in blockchain.chain[1:]:
                if name in student.data.full_name:
                    print(student.to_string())
                    return
            else:
                print("No student found with name: " + name + "\n")


class Slot:
    def __init__(self, course_name, class_number, slot, venue, faculty):
        self.course_name = course_name
        self.class_number = class_number
        self.slot = slot
        self.venue = venue
        self.faculty_name = faculty
        self.students_list = []

    def to_string(self):
        return f"Course Name: {self.course_name}\nClass Number: {self.class_number}\nSlot: {self.slot}\n" \
               f"Venue: {self.venue}\nFaculty: {self.faculty_name}\n"

    def serialize(self):
        return f"{self.course_name},{self.class_number},{self.slot},{self.venue},{self.faculty_name}"

    @staticmethod
    def write_to_file(blockchain):
        write_file = open("slots.csv", "w")
        data = ""
        for block in blockchain.chain[1:]:
            data += block.data.serialize()

            if block is not blockchain.last_block:
                data += ','
        write_file.write(data)

    def assign_students(self, student_blockchain):
        for block in student_blockchain.chain[1:]:
            self.students_list.append(block.data)

    @staticmethod
    def read_from_file(slots_blockchain, student_blockchain):
        read_file = open("slots.csv")

        for line in read_file:
            data = line.rstrip('\n').split(',')

            course_id = data[0]
            data.pop(0)

            class_number = data[0]
            data.pop(0)

            slot = data[0]
            data.pop(0)

            venue = data[0]
            data.pop(0)

            faculty = data[0]
            data.pop(0)

            slot = Slot(course_id, class_number, slot, venue, faculty)
            slot.assign_students(student_blockchain)
            slot_block = Block(slots_blockchain.last_block.index + 1, slot, slots_blockchain.last_block.hash)
            slots_blockchain.add_to_chain(slot_block)

    @staticmethod
    def search_slots(blockchain):
        print("1. Search slots by course name")
        print("2. Search slots by class number")

        choice = int(input("Enter you choice "))

        if choice == 1:
            course_name = input("Enter the course name you want to search: ")
            for course in blockchain.chain[1:]:
                if course.data.course_name == course_name:
                    print(course.to_string())
                    return
            print("No course with course name " + course_name + " found")

        elif choice == 2:
            class_number = input("Enter the class number you want to search: ")
            for course in blockchain.chain[1:]:
                if course.data.class_number == class_number:
                    print(course.to_string())
                    return
            print("No class with class number " + class_number + " found")


class Faculty:
    def __init__(self, name, department, email, cabin):
        self.name = name
        self.department = department
        self.email = email
        self.cabin = cabin

    def to_string(self):
        return f"Name: {self.name}\nDepartment: {self.department}\nE-Mail: {self.email}\nCabin: {self.cabin}\n"

    def serialize(self):
        return f"{self.name},{self.department},{self.email},{self.cabin}"

    @staticmethod
    def write_to_file(blockchain):
        write_file = open("faculty.csv", "w")
        data = ""
        for block in blockchain.chain[1:]:
            data += block.data.serialize()

            if block is not blockchain.last_block:
                data += ','

        write_file.write(data)

    @staticmethod
    def read_from_file(blockchain):
        read_file = open("faculty.csv", "r")

        for line in read_file:
            data = line.rstrip('\n').split(',')

            name = data[0]
            data.pop(0)

            department = data[0]
            data.pop(0)

            email = data[0]
            data.pop(0)

            cabin = data[0]
            data.pop(0)

            faculty = Faculty(name, department, email, cabin)
            new_faculty_block = Block(blockchain.last_block.index + 1, faculty, blockchain.last_block.hash)
            blockchain.add_to_chain(new_faculty_block)

    @staticmethod
    def search_faculty(blockchain):
        name = input("Enter the name of the faculty you wish to search for: ")
        for faculty in blockchain.chain[1:]:
            if name in faculty.data.name:
                print(faculty.to_string())
                break
        print("No faculty with name " + name + " found")
