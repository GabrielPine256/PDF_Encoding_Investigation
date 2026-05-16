# PDF_Encoding_Investigation

I downloaded the PDF at the publicly accessible URL https://hov.to/fb0f62b9 . As I read through, I kept trying to copy out segments to my own notes for comments, review, and highlighting. Each time I did this, I got a string of ASCII symbols (few alphabetic or digit characters). 

A text file contains a string of characters. A character is stored in 8 bits. (An integer conventionally uses 32.) Your .txt files will be as many bytes as characters you type into them, with an additional 00000000 byte as a termination signal. (The binary value 00000000 is represented by the character '\0' in C.) 

At first I suspected it was a bitshift. If you read 8 bit increments, starting from not a multiple of 8, the result would be nonsense. Maybe a primitive anti-piracy measure, maybe an implementation detail. 

Looking it up, I found talk of PDF character encodings not matching up, and trying a different PDF reader. (My PDF reader was displaying the plaintext just fine. The copy-paste was producing something else.) I could either download PDF readers until one happened to work, or I could decrypt the encoding directly. 

---

The PDF's metadata includes the following: 
Producer:   Mac OS X 10.5.8 Quartz PDFContext 
Created:    Wed 21 Oct 2009 08:41:58 PM UTC 
Modified:   Wed 21 Oct 2009 08:41:58 PM UTC 
Format:     PDF-1.3

When I found 'w' always copied to '$', and ' ' always copied to '(', I decided to treat it as a MonoAlphabetic Substitution Cipher. One of the first C programs I wrote, after the exercises in K&R, was an attempt to automate the breaking of MonoAlphabetic Substitution Ciphers using linguistic analysis. This time around though, I have the plaintext. I can manually transcribe passages of the text to provide samples. The program in the PDF_Decoder folder was written to compare the two samples, character by character, and list correspondences. I implemented a map of characters to sets of characters, in case multiple correspondences were found. (Whether my initial impression that a 1:1 correspondence wasn't occurring was in fact true, or to absorb mistakes in aligning the samples.) 

I intended to post this as a general use program to figure out arbitrary PDF encodings, so I gave the user the ability to specify the names of files to read. After a few syntax errors were debugged, and copying an argument-reading library I wrote for my CLI board-games, it worked. ... And it showed chaos. Many characters corresponded to a spread of characters, and these spreads were quite similar. 

---

I'll fall back on my bitshifting instinct. I want to display the 1s and 0s of the two sample files in a neat side-by-side way. The important lines of code (char to bitset) will be easy enough to look through. This is a text manipulation exercise, like the K&R book was full of. Rather than waste time on a hunch making this nice display code, I have Claude write it. (Though, I did clean it up some before compiling.) My hunch was wrong. These are not the same bitstream. 

---

The LLM points out "PREFACE" in the plaintext is 'encoded' as consecutive ASCII characters. What I had been looking at was not ciphertext, but a list of indexes to an encoding stored elsewhere in the PDF. The information of the plaintext is not to be found in what I had been working on. 
It offers the following tools to figure the encoding of the PDF: 

$ strings output.pdf | grep -i "encoding" 

Which gives: 
<< /BaseFont /NFMIKD+AmericanTypewriter-Bold /Encoding /MacRomanEncoding /FirstChar 32 /FontDescriptor 77 0 R /LastChar 222 /Subtype /TrueType /Type /Font /Widths 78 0 R >>
<< /BaseFont /BBJMGB+AmericanTypewriter /Encoding /MacRomanEncoding /FirstChar 32 /FontDescriptor 79 0 R /LastChar 211 /Subtype /TrueType /Type /Font /Widths 80 0 R >>
<< /BaseFont /NLTNUD+Symbol /Encoding /MacRomanEncoding /FirstChar 165 /FontDescriptor 96 0 R /LastChar 165 /Subtype /TrueType /Type /Font /Widths 97 0 R >>

-

$ pdftotext -enc MacRoman input.pdf output.txt

And then letting it auto-detect the encoding: 

$ pdftotext input.pdf output.txt

-

$ qpdf --decrypt input.pdf output.pdf

$ pdftohtml output.pdf output.html 

-

$ pipx install pdfminer.six

$ pdf2txt.py output.pdf

-

Claude: 
> **Mac OS X 10.5.8's Quartz PDFContext** (2009) had known issues:
> - It created subset fonts with `MacRomanEncoding` but didn't reliably embed ToUnicode CMaps
> - Even if a CMap existed, it might have been generated incorrectly
> - When copy-pasting, older viewers would use the font encoding table directly instead of the proper Unicode mapping

$ qpdf --stream-data=preserve output.pdf repaired.pdf

-

$ gs -sDEVICE=txtwrite -o plaintext.txt output.pdf

& gs -sDEVICE=txtwrite -dUseCIEColor -o plaintext.txt output.pdf

-

All output text files included the first page itself (which actually copy-pastes fine) and gibberish for the rest. 

---

It falls back to suggesting I use OCR. If the encoding cannot be recovered, AI can "visually" look through the displayed PDF pages. 
This is a PDF I found on the public internet, so I needn't worry about leaking the contents to any public sites. 

https://ocr.space/ 
On the highest fidelity setting, it returned only 2 of 19 pages. 
On the lower fidelity setting, it returned 10 of 19 pages. 

https://www.ilovepdf.com/ocr    
This link is dead. 
Clicking around though, I found:    
https://www.ilovepdf.com/pdf_to_word    

And LibreOffice had no trouble converting .docx to .txt . 

---

I was left throwing my hands up. A PDF was made in 2009, on Mac OS X. That OS uses FreeBSD, which is from 1993. ASCII is from 1963. 

These are software and computer data. Computer programmers, of all people, aren't the type to leave inefficiency in their systems. So why is the text split across two areas of the PDF and not a conventional stream of bytes? 

A couple web searches later, I am reminded that ASCII is a 7-bit encoding (for 128 characters). Meaning that given byte alignment, they take 8/7 the space they use. 
Additionally, only 95 of the 128 are printable. (The rest are control characters, such as '\0', Bell, SYN, and ACK.) 6 bits can store 64 states. 7 bits can store 128 states. To use ACSII encoding would mean using the space of 256 states for 95. And so, PDFs use custom character encodings. 
