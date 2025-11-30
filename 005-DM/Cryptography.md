### **I. Core Terminology**

**Cryptography** is the study of how to safely convert readable information into an unreadable form and then recover it again.
Key terms you need to keep straight:

* **Cipher:** The specific algorithm or rule used to perform the encoding.
* **Plaintext:** The original readable message.
* **Ciphertext:** The transformed, unreadable message.
* **Encryption:** Turning plaintext into ciphertext.
* **Decryption:** Turning ciphertext back into plaintext.

---

### **II. Classical Cryptography**

#### **1. Caesar Cipher**

A simple substitution technique attributed to Julius Caesar.
Its mechanism is straightforward: every letter in the message is shifted 3 positions forward in the alphabet, looping around when needed (X→A, Y→B, Z→C).

The math behind it:

* Assign letters numerical values from 0 to 25.
* Encryption:           **f(p) = (p + 3) mod 26**
* Decryption:           **f⁻¹(p) = (p − 3) mod 26**

#### **2. Shift Cipher (General Case)**

The Caesar cipher is just a shift cipher with a key of 3.
A shift cipher uses any integer key **k** to determine how far each letter moves.

* Encryption:       **f(p) = (p + k) mod 26**
* Decryption:       **f⁻¹(p) = (p − k) mod 26**

Example: With **k = 11**, the word “STOP” becomes “DEZA.”

---

### **III. Cryptosystems & Communication Models**

#### **1. Communication Flow**

Cryptography is often explained using three characters:

* **Alice** (sender)
* **Bob** (receiver)
* **Eve** (the eavesdropper)

The goal is simple: Alice wants to send Bob a message that Eve cannot make sense of.

#### **2. Types of Cryptosystems**

**Symmetric Cryptosystems**
Alice and Bob share one secret key **K**. The same key is used for both encryption and decryption. AES is a well-known example.

**Public-Key (Asymmetric) Cryptosystems**
No shared secret is required beforehand. Instead, each participant has:

* a **public key** (used by anyone to encrypt), and
* a **private key** (used only by the owner to decrypt).

Knowing the public key doesn’t help Eve decrypt anything. The private key does all the heavy lifting.

---

### **IV. The RSA Cryptosystem**

RSA is the classic example of a public-key cryptosystem, built on the difficulty of factoring large numbers.

#### **1. Key Generation**

1. Choose two large prime numbers **p** and **q**.
2. Compute the modulus:  **n = p × q**
3. Compute Euler’s totient: **k = (p − 1)(q − 1)**
4. Pick a public exponent **e**, where **1 < e < k** and **gcd(e, k) = 1**.
   The public key is **(n, e)**.
5. Compute **d**, the modular inverse of **e** modulo **k**, so that
       **d × e ≡ 1 (mod k)**
   The private key is **(n, d)**.

#### **2. Operations**

**Encryption:**
Convert the message **M** into an integer **m**, then compute
    **c ≡ mᵉ mod n**

**Decryption:**
Recover the message using
    **m ≡ cᵈ mod n**

**Digital Signatures:**
Signing uses the private key:  **s = mᵈ mod n**
Verification uses the public key: **m = sᵉ mod n**

#### **3. Example**

Let **p = 7** and **q = 17**, giving **n = 119**.
Totient: **k = 96**.
Pick **e = 5**. Compute **d = 77**, since **5 × 77 ≡ 1 (mod 96)**.

Encrypting the message 19:
 Ciphertext = **19⁵ mod 119 = 66**
Decrypting:      **66⁷⁷ mod 119 = 19**

---

### **V. Mathematical Foundation**

#### **Fermat’s Little Theorem**

A key tool for handling giant exponents in RSA.

If **p** is prime and **a** is not divisible by **p**, then
    **a^(p−1) ≡ 1 (mod p)**

Equivalent form:
    **aᵖ ≡ a (mod p)**

Example: Compute **7²²² mod 11**.
Since **7¹⁰ ≡ 1 (mod 11)**, rewrite 222 as **22×10 + 2**.
Thus the remainder is simply **7² = 49 ≡ 5 (mod 11)**.

---

If you want, I can help turn this into an exam-friendly cheat sheet, a lecture summary, or a printable one-page reference.
