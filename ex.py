x = 0  # Position initiale
v = 0  # Vitesse initiale
a = 2  # Accélération initiale
dt = 1  # Intervalle de temps

for t in range(100):
    if t == 10:
        a = 0  # On arrête d’accélérer à t = 10
    if t == 16:
        a = -1

    x = x + v * dt + 0.5 * a * dt**2
    v = v + a * dt  # Après t = 10, v reste constant

    print(f"Temps: {t+1}s, Position: {x}, Vitesse: {v}, Accélération: {a}")
