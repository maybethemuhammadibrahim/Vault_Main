
### **I. Core Terminology**
* [cite_start]**Cryptography:** The science of encoding and decoding messages[cite: 9].
* **Key Terms:**
    * [cite_start]**Cipher:** The specific method or algorithm used for encoding[cite: 10].
    * [cite_start]**Plaintext:** The original, readable message[cite: 11].
    * [cite_start]**Ciphertext:** The encoded, unreadable message[cite: 12].
    * [cite_start]**Encryption:** The process of converting plaintext to ciphertext[cite: 13].
    * [cite_start]**Decryption:** The process of converting ciphertext back to plaintext[cite: 14].

---

### **II. Classical Cryptography**
#### **1. Caesar Cipher**
A substitution method created by Julius Caesar.
* [cite_start]**Mechanism:** Shifts every letter forward by 3 positions in the alphabet (wrapping XYZ to ABC)[cite: 16, 22].
* **Mathematical Process:**
    * [cite_start]Map letters to integers $0$ to $25$ (A=0, B=1... Z=25)[cite: 24].
    * [cite_start]**Encryption Function:** $f(p) = (p + 3) \mod 26$[cite: 25].
    * [cite_start]**Decryption Function:** $f^{-1}(p) = (p - 3) \mod 26$[cite: 32].

#### **2. Shift Cipher (Generalization)**
The Caesar cipher is a specific type of **Shift Cipher** where the key is 3.
* [cite_start]**Key ($k$):** An integer used to determine the shift amount[cite: 38].
* [cite_start]**Encryption:** $f(p) = (p + k) \mod 26$[cite: 36].
* [cite_start]**Decryption:** $f^{-1}(p) = (p - k) \mod 26$[cite: 37].
* [cite_start]**Example:** With $k=11$, "STOP" becomes "DEZA"[cite: 40, 45].

---

### **III. Cryptosystems & Communication Models**
#### **1. Communication Flow**
* [cite_start]**Participants:** Alice (sender), Bob (receiver), and Eve (eavesdropper/interceptor)[cite: 53, 58, 59].
* [cite_start]**Goal:** Secure communication so Eve cannot read the content[cite: 76].

#### **2. Types of Cryptosystems**
* **Symmetric Cryptosystems:**
    * [cite_start]Alice and Bob share a **single secret key ($K$)**[cite: 77].
    * [cite_start]The same key is used to encrypt and decrypt ($encrypt(m, K)$ and $decrypt(c, K)$)[cite: 78, 79].
    * [cite_start]*Example:* AES[cite: 80].
* **Public Key Cryptosystems (Asymmetric):**
    * [cite_start]No shared secret is required beforehand[cite: 81].
    * Uses a **Key Pair**:
        1.  [cite_start]**Public Key:** Known to everyone; used to **encrypt** messages[cite: 83, 84].
        2.  [cite_start]**Private Key:** Known only to the recipient (Bob); used to **decrypt** messages[cite: 83, 85].
    * [cite_start]**Security:** Even if Eve knows the public key, she cannot decrypt the message without the private key[cite: 72].

---

### **IV. The RSA Cryptosystem**
A widely used public-key algorithm based on number theory.

#### **1. Key Generation Process**
1.  [cite_start]**Choose Primes:** Select two distinct random prime numbers, $p$ and $q$[cite: 88].
2.  [cite_start]**Compute Modulus:** Calculate $n = p \times q$[cite: 90].
3.  [cite_start]**Compute Totient:** Calculate $k = (p-1)(q-1)$[cite: 90].
4.  [cite_start]**Select Public Exponent ($e$):** Choose $e$ such that $1 < e < k$ and $gcd(e, k) = 1$ (they are coprime)[cite: 91].
    * [cite_start]**Public Key:** $(n, e)$[cite: 95].
5.  **Compute Private Exponent ($d$):** Calculate $d$ as the multiplicative inverse of $e$ modulo $k$.
    * [cite_start]Formula: $d \times e \equiv 1 \mod k$[cite: 93, 94].
    * [cite_start]**Private Key:** $(n, d)$[cite: 95].

#### **2. Operations**
* **Encryption (by Sender):**
    * [cite_start]Convert message $M$ to integer $m$ where $0 \le m < n$[cite: 100].
    * [cite_start]Compute ciphertext $c$: $$c \equiv m^e \mod n$$[cite: 102].
* **Decryption (by Receiver):**
    * [cite_start]Recover message $m$: $$m \equiv c^d \mod n$$[cite: 104].
* **Digital Signatures:**
    * [cite_start]To sign, the sender computes $s = m^d \mod n$[cite: 113].
    * [cite_start]To verify, the receiver computes $m = s^e \mod n$[cite: 114].

#### **3. Example Calculation**
* **Given:** $p=7, q=17 \rightarrow n=119$.
* [cite_start]**Totient:** $k = (6)(16) = 96$[cite: 119].
* **Keys:** Select $e=5$. [cite_start]Compute $d=77$ (since $5 \times 77 \pmod{96} = 1$)[cite: 121].
* **Action:** Encrypt message $19$.
    * [cite_start]$19^5 \mod 119 = 66$ (Ciphertext)[cite: 125].
    * [cite_start]$66^{77} \mod 119 = 19$ (Decrypted)[cite: 126].

---

### **V. Mathematical Foundation**
#### **Fermat's Little Theorem**
Used to compute remainders of large powers efficiently, which is critical for RSA.
* **Theorem:** If $p$ is a prime and $a$ is an integer not divisible by $p$:
    [cite_start]$$a^{p-1} \equiv 1 \pmod p$$[cite: 155].
* [cite_start]**Alternative Form:** For any integer $a$: $a^p \equiv a \pmod p$[cite: 156].
* **Application Example:** Find $7^{222} \mod 11$.
    * We know $7^{10} \equiv 1 \pmod{11}$.
    * Break down power: $222 = (22 \times 10) + 2$.
    * [cite_start]Result: $7^2 = 49 \equiv 5 \pmod{11}$ [cite: 159-163].
