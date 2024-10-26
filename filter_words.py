words = input("Wörter: ").split(" ")
# Einfachster Weg wegen Duplikaten
r = []
for w in words:
    w = w.replace("ß", "ss")
    if "ä" in w or "ö" in w or "ü" in w:
        continue
    if len(w) == 5:
        r.append(w)
        
for w in r:
    print(w)