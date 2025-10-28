https://tihldestorage.blob.core.windows.net/applicationpdf/1e4b5000-ba1b-4cb1-a357-9c2861672418Operativsystemer-Pensum.pdf 

**Salt:** random string of characters that is added to the password before hashing and works as a safeguard for stored passwords. The salt is meant to ensure that the hash is unique compared with another user who has the same hashed password. 
### 12.1 Security objectives and principle
Confidentiality
Integrity
Availability
Accountability
### 12.2 User authentication
Potensial weakness for basic authentication; logging into a computer using a password at the start of the session usage.

#### 12.2.1 Password Capture Using Spoofing and Phishing

#### 12.2.2 Checking password without storing them
To avoid storing password, a cryptographic hash function is used.
System feed a newly chosen password through the hash function and store the hash value

When user tries to log in, the system feeds the proffered password through the same hash function and compares the resulting value with the value with the stored hash code. If the hash codes are equal, the system is sure that the correct password was entered.
#### 12.2.3 Passwords for Multiple, Independent Systems
Password wallet: stores a range of passwords under the protection of one master password. 
Another technique: Individual passwords are generated algorithmically from the master password and site's name. But no easy way to change the master password. 
#### 12.2.4 2FA

### 12.3 Access and information-flow controls

### 12.4 Viruses and worms

### 12.5 Security assurance

### Difference Between Microkernel and Monolithic Kernel
- Two ways of organizing and managing the core functions of a computer's OS.
- **Kernel:** computer program at the core of a computer's OS and has complete control over everything in the system. Manages the operation of the computer and hardware
- A Microkernel, which only contains basic functionality
- A Monolithic Kernel, which contains many device drivers

**Microkernel**
- User services and kernel services are implemented in different address spaces. 
- Only the most essential functions are included in the kernel, such as basic communication between hardware and software and simple process management
- Other services like device drivers, file systems, and network protocols are run in users space as separate processes
- Advantages:
	- Less prone to bugs and errors because of its simplicity
- Disadvantages:

**Monolithic kernel**
- The entire OS runs as a single program in a single address space in kernel mode. 
- User services and kernel service implemented in the same address space.
- Advantage:
	- Faster because all components can interact directly
- Disadvantages
	- Because all components are integrated into a program, it can make the system more complex and harder to maintain, as a bug in one part of the kernel can potentially affect the entire system. 
	- More prone to errors and bugs do to its 
