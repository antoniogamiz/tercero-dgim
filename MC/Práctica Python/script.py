# coding=utf-8

from afd2 import afd
from utils import rf

aut1 = afd(rf("aut2.af"))
aut2 = afd(rf("aut.af"))

# aut1.automatonProduct(aut2)
# aut1.intersection(aut2)
# aut1.union(aut2)
# aut1.complementary()

aut1.minimize()

print("Done.")