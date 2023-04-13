import numpy as np
import scipy.optimize as opt


def f(xvec):
    x, y = xvec
    return np.array([
        x + 2*y - 2,
        x**2 + 4*y**2 - 4
    ])

def Jf(xvec):
    x, y = xvec
    return np.array([
        [1, 2],
        [2*x, 8*y]
    ])

x0 = [1, 1]
x = x0
tol = 1e-8
max_iter = 100
iter_count = 0

for i in range(max_iter):
    fx = f(x)
    Jfx = Jf(x)
    delta_x = np.linalg.solve(Jfx, -fx)
    x = x + delta_x
    if np.linalg.norm(delta_x) < tol:
        break
    iter_count += 1

print("Solution:", x)
print("Iterations:", iter_count)