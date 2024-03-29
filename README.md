# Κ23γ: Ανάπτυξη Λογισμικού για Αλγοριθμικά Προβλήματα

**Χειμερινό εξάμηνο 2022-23**
- Σακαρέλλος Ιωάννης (sdi1800167)
- Μασούρας Νικόλαος (sdi1800112)

# 1<sup>η</sup> Προγραμματιστική Εργασία

## Περιγραφή εργασίας

**Πολυγωνοποίηση σημειοσυνόλου με τη χρήση της βιβλιοθήκης CGAL (C++)**

Το πρόγραμμα `to_polygon` λειτουργεί και δίνει σωστά αποτελέσματα για τα παραδείγματα στα οποία δοκιμάστηκε. Σε ορισμένα, όμως, καθυστερεί αρκετά (ειδικά σε αυτά με πολύ μεγάλο όγκο δεδομένων), ενώ σε άλλα (ακόμα και με 1000 ή 2000 σημεία) όχι. Αυτό ίσως συμβαίνει γιατί οι αλγόριθμοι μας δεν έχουν την καλύτερη δυνατή πολυπλοκότητα, αφού προτεραιότητά μας ήταν η κατανόηση της βιβλιοθήκης `CGAL` και των αλγορίθμων `Incremental` και `Convex Hull`, και όχι η βελτιστοποίηση του κώδικά τους. Πέραν αυτού, όμως, παρατηρήσαμε ότι ο αλγόριθμος `Convex Hull` κατά κύριο λόγο αργεί περισσότερο από τον `Incremental` (ακόμα και στα ίδια παραδείγματα). Αυτό συμβαίνει λόγω της φύσης του αλγορίθμου και είναι ανεξάρτητο της υλοποίησης.

## Οδηγίες μεταγλώττισης

Για την παραγωγή του εκτελέσιμου αρχείου `to_polygon`, ακολουθούμε τα εξής βήματα:
- Μεταβαίνουμε στον φάκελο `build`
- Δίνουμε την εντολή `cmake ..`
- Δίνουμε την εντολή `make`
- Μεταβαίνουμε στον φάκελο `main` που δημιουργήθηκε
- Εκτελούμε το τελικό εκτελέσιμο αρχείο με την εντολή `./to_polygon` ακολουθούμενη από τα κατάλληλα ορίσματα

## Οδηγίες χρήσης

Η εκτέλεση του προγράμματος γίνεται μέσω της εντολής `$./to_polygon -i <input_file> -o <output_file> -algorithm <algorithm> -edge_selection <edge_selection> -initialization <initialization>`, όπου:
- Το `<input_file>` είναι το όνομα του αρχείου που περιέχει τα δεδομένα εισόδου
- Το `<output_file>` είναι το όνομα του αρχείου στο ποίο θα γραφούν τα δεδομένα εξόδου. **Προσοχή**: Το αρχείο `<output_file>` δεν πρέπει να υπάρχει ήδη
- Το `<algorithm>` πρέπει να είναι `incremental` ή `convex_hull` ανάλογα με τον αλγόριθμο που πρέπει να εκτελεστεί
- Το `<edge_selection>` πρέπει να είναι `1`, `2` ή `3` ανάλογα με τον τρόπο που πρέπει να επιλέγεται μία εκ των ορατών ακμών σε κάθε βήμα. Συγκεκριμένα:
    - `1`: Τυχαία επιλογή ορατής ακμής
    - `2`: Επιλογή ορατής ακμής έτσι ώστε να προστίθεται ελάχιστον εμβαδό
    - `3`: Επιλογή ορατής ακμής έτσι ώστε να προστίθεται μέγιστο εμβαδό
- Το `<initialization>` πρέπει να υπάρχει **μόνο σε συνδυασμό με τον αλγόριθμο `incremental`** και πρέπει να είναι `1a`, `1b`, `2a` ή `2b` ανάλογα με την επιθυμητή αρχική ταξινόμηση των σημείων στον Αυξητικό Αλγόριθμο. Συγκεκριμένα:
    - `1a`: Φθίνουσα ταξινόμηση ως προς `x`
    - `1b`: Αύξουσα ταξινόμηση ως προς `x`
    - `2a`: Φθίνουσα ταξινόμηση ως προς `y`
    - `2b`: Αύξουσα ταξινόμηση ως προς `y`

## Περιγραφή αρχείων κώδικα

### Βιβλιοθήκη `arguments`

Η βιβλιοθήκη `arguments` αποτελείται από τα αρχεία `arguments.cpp` και `arguments.hpp`. Υλοποιεί την κλάση που διαβάζει, επαληθεύει και αποθηκεύει τα ορίσματα που δίνει ο χρήστης κατά την εκτέλεση του προγράμματος `to_polygon`. Συγκεκριμένα, αποθηκεύει:
- Το όνομα του αρχείου με τα σημεία προς επεξεργασία (`in_file`)
- Το όνομα του αρχείου όπου θα γραφτεί η έξοδος του προγράμματος (`out_file`)
- Τον αλγόριθμο που θα χρησιμοποιηθεί (`incremental` ή `convex_hull`)
- Τον τρόπο με τον οποίο θα επιλέγεται μία από τις ορατές ακμές σε κάθε βήμα (`1`, `2` ή `3`)
- Τον τρόπο με τον οποίο θα ταξινομούνται τα σημεία (`1a`, `1b`, `2a` ή `2b`) - μόνο στον αυξητικό αλγόριθμο
- Ένα `vector` με όλα τα σημεία που διαβάστηκαν από το αρχείο `in_file`

### Βιβλιοθήκη `polyline`

Η βιβλιοθήκη `polyline` αποτελείται από τα αρχεία `polyline.cpp` και `polyline.hpp`. Υλοποιεί την κλάση `polyline`, μέσω της οποίας γίνεται όλη η κατασκευή της ζητούμενης πολυγωνικής γραμμής. Συγκεκριμένα:
- Κατά την κλήση του `constructor` της κλάσης, αποθηκέυονται στις αντίστοιχες μεταβλητές της κλάσης οι απαιτούμενες τιμες και καλείται η κατάλληλη συνάρτηση (`incremental()` ή `convex_hull()` ανάλογα)
- Μετά το πέρας της επεξεργασίας των δεδομένων από τη σωστή συνάρτηση, τα δεδομένα εξόδου γράφονται στο αρχείο `out_file`

### Βοηθητικές συναρτήσεις

Κατά τη διάρκεια εκτέλεσης του προγράμματος, χρησιμοποιούνται κάποιες `private` βοηθητικές συναρτήσεις. Αυτές είναι:
- `sort_points()`: Ταξινομεί τα σημεία που δόθηκαν ως αρχική είσοδος
- `init_triangle()`: Αρχικοποιεί την πολυγωνική γραμμή με τον μικρότερο δυνατό αριθμό μη συνευθειακών σημέιων. Επιστρέφει τον αριθμό των σημείων που χρειάστηκαν
- `expand()`: Ξεκινώντας από το σημείο που της δόθηκε ως όρισμα, μεγαλώνει την πολυγωνική γραμμή ακολουθώντας τα βήματα του Αυξητικού Αλγορίθμου
- `get_ch()`: Επιστρέφει τις κορυφές της πολυγωνικής γραμμής που αποτελεί το Κυρτό Περίβλημα των σημείων που της δόθηκαν ως είσοδος
- `get_segment()`: Επιστρέφει τις ακμές που σχηματίζουν τα σημεία που της δόθηκαν ως είσοδος (με τη σειρά αυτή)
- `get_red_edges()`: Επιστρέφει τις ακμές που βρίσκονταν στο προηγούμενο Κυρτό Περίβλημα (πρώτο όρισμα), αλλά όχι στο τωρινό (δεύτερο όρισμα). Οι ακμές αυτές είναι οι ζητούμενες `red edges`
- `get_vis_edges()`: Επιστρέφει τις `visible edges` ως προς ένα συγκεκριμένο σημείο (πρώτο όρισμα), από αυτές που δόθηκαν ως δεύτερο όρισμα (μόνο κάποιες `red edges` ή κάποιες ακμές "πίσω" από αυτές μπορεί να είναι `visible edges`)
- `is_vis()`: Επιστρέφει `true` αν και οι δύο ακμές που δόθηκαν ως όρισμα δεν τέμνονται από καμία ακμή της πολυγωνικής γραμμής, σε διαφορετική περίπτωση επιστρέφει `false`
- `min_area()`: Επιστρέφει μία ακμή από το σύνολο αυτών που της δόθηκαν ως πρώτο όρισμα. Η ακμή αυτή πρέπει να αντικατασταθεί για να επιτευχθεί όσο το δυνατόν μικρότερη αύξηση του εμβαδού της πολυγωνικής γραμμής
- `max_area()`: Επιστρέφει μία ακμή από το σύνολο αυτών που της δόθηκαν ως πρώτο όρισμα. Η ακμή αυτή πρέπει να αντικατασταθεί για να επιτευχθεί όσο το δυνατόν μεγαλύτερη αύξηση του εμβαδού της πολυγωνικής γραμμής
- `write_to_file()`: Γράφει τα δεδομένα εξόδου στο αρχείο `out_file` (το αρχείο πρέπει να μην υπάρχει ήδη)
