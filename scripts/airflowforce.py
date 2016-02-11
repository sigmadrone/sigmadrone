import numpy as np

class AirflowForce:
    def __init__(self, diameter, maxthrust):
        self.Ro = 1.225
        self.diameter = diameter
        self.half_Ro_A = 0.5 * self.Ro * np.pi * (diameter * diameter / 4.0)
        self.Fmax = maxthrust
        self.Vmax = np.sqrt(self.Fmax / self.half_Ro_A)
        
    def force(self, Vout, Vin = 0.0):
        return self.half_Ro_A * (Vout - Vin) * (Vout + Vin);

    def velocity_delta(self, deltaF, Vout, Vin = 0.0):
        F = self.force(Vout, Vin) if (Vout > 0) else 0.0;
        Fnew = F + deltaF if (F + deltaF > 0) else 0.0;
        c = Vout * Vout - (Fnew + self.half_Ro_A * Vin * Vin) / self.half_Ro_A;
        b = 2.0 * Vout;
        a = 1.0;
        Det = np.sqrt(b * b - 4.0 * a * c);
        x1 = 0.5 * (-b + Det) / a;
        return x1;
    
    def velocity(self, F, Vin = 0.0):
        if F < 0:
            return 0;
        return np.sqrt(Vin + F / self.half_Ro_A);
    
    def nforce(self, Vout, Vin = 0.0):
        return self.force(Vout * self.Vmax, Vin * self.Vmax) / self.Fmax;

    def nvelocity_delta(self, deltaF, Vout, Vin = 0.0):
        return self.velocity_delta(deltaF * self.Fmax, Vout * self.Vmax, Vin * self.Vmax) / self.Vmax;

    def nvelocity(self, F, Vin = 0.0):
        return self.velocity(F * self.Fmax, Vin * self.Vmax) / self.Vmax;

pf = AirflowForce(0.24, 1.77 * 0.59)

print((pf.force(2.0), pf.force(4.0), pf.force(6.0), pf.force(8.0), pf.force(10.0)))
print((pf.nforce(.2), pf.nforce(.4), pf.nforce(.6), pf.nforce(.8), pf.nforce(1)))
print((pf.velocity_delta(0.1, 2), pf.velocity_delta(0.1, 4), pf.velocity_delta(0.1, 6), pf.velocity_delta(0.1, 8), pf.velocity_delta(0.1, 10)))
print((pf.nvelocity_delta(0.1, .2), pf.nvelocity_delta(0.1, .4), pf.nvelocity_delta(0.1, .6), pf.nvelocity_delta(0.1, .8), pf.nvelocity_delta(0.1, 1)))
print(pf.nvelocity_delta(-0.1, .31623))

import matplotlib.pyplot as plt
x = np.linspace(0, 1, 500)
V = []
for f in x:
    V.append(pf.nvelocity(f))
plt.plot(x, V)
plt.title("Propeler Force")
plt.show()

