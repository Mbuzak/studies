### Files
1. Szyfrowanie: "plain.txt" + "key.txt" -> "crypto.txt"
2. Odszyfrowywanie: "crypto.txt" + "key.txt" -> "decrypt.txt"
3. Kryptoanaliza z użyciem tekstu jawnego: "crypto.txt" + "extra.txt" -> "keyfound.txt" + -> "decrypt.txt"
4. Kryptoanaliza w oparciu o kryptogram: "crypto.txt" -> "decrypt.txt"

### Execution
```
python cezar.py
```

### 90%
program should have parameters instead input() operation