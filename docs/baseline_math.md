# Fixed Spread Baseline Mathematics

This document derives the expected execution edge for the fixed spread strategy in the original market model.

The purpose of this derivation is to calculate what the simulator should produce before comparing the mathematical prediction with the repeated simulation results.

---

## 1. Model Setup

Let

\[
V
\]

be the hidden true value of the asset.

The market maker does not observe \(V\) directly. Instead, it observes a noisy signal

\[
S = V + \epsilon,
\]

where

\[
\epsilon \sim N(0,\sigma^2).
\]

Here, \(\epsilon\) is the realized signal error at a particular timestep, while \(\sigma\) controls how noisy the signal is overall.

The fixed spread strategy places its quotes a constant distance \(h\) from the observed signal:

\[
\text{bid} = S-h
\]

and

\[
\text{ask} = S+h.
\]

Substituting \(S=V+\epsilon\),

\[
\text{bid}=V+\epsilon-h
\]

and

\[
\text{ask}=V+\epsilon+h.
\]

In the baseline experiments,

\[
h=1.
\]

The arriving trader is informed with probability

\[
p_I=0.40.
\]

Otherwise, the arriving trader is a noise trader.

A noise trader trades with probability

\[
q=0.30
\]

and, conditional on trading, buys or sells with equal probability.

Although \(V\) changes over time in the simulator, the fixed-spread execution-edge calculation depends only on the signal error

\[
\epsilon=S-V,
\]

because \(V\) cancels when the bid and ask are compared with the true value.

---

## 2. When Does an Informed Trader Trade?

The informed trader knows the true value \(V\) and only trades when one of the market maker's quotes is favorable.

### Informed Buy

The informed trader buys from the market maker when

\[
\text{ask}<V.
\]

Since

\[
\text{ask}=V+\epsilon+h,
\]

we have

\[
V+\epsilon+h<V.
\]

Subtracting \(V\),

\[
\epsilon+h<0,
\]

so

\[
\boxed{\epsilon<-h}.
\]

### Informed Sell

The informed trader sells to the market maker when

\[
\text{bid}>V.
\]

Since

\[
\text{bid}=V+\epsilon-h,
\]

we have

\[
V+\epsilon-h>V.
\]

Subtracting \(V\),

\[
\epsilon-h>0,
\]

so

\[
\boxed{\epsilon>h}.
\]

Therefore an informed trade occurs exactly when

\[
\boxed{|\epsilon|>h}.
\]

---

## 3. Execution Edge

Execution edge is measured from the market maker's perspective.

If the counterparty buys from the market maker,

\[
\text{edge}=\text{ask}-V.
\]

If the counterparty sells to the market maker,

\[
\text{edge}=V-\text{bid}.
\]

A positive value is favorable to the market maker and a negative value is unfavorable.

### Informed Buy Edge

For an informed buy,

\[
\text{edge}=\text{ask}-V.
\]

Substituting the ask,

\[
\text{edge}
=
(V+\epsilon+h)-V.
\]

Therefore,

\[
\text{edge}=h+\epsilon.
\]

But an informed buy only occurs when

\[
\epsilon<-h,
\]

so \(\epsilon\) is negative.

For negative \(\epsilon\),

\[
|\epsilon|=-\epsilon.
\]

Therefore,

\[
\epsilon=-|\epsilon|.
\]

Substituting this into the edge,

\[
h+\epsilon
=
h-|\epsilon|.
\]

Thus,

\[
\boxed{\text{edge}=h-|\epsilon|}
\]

for an informed buy.

### Informed Sell Edge

For an informed sell,

\[
\text{edge}=V-\text{bid}.
\]

Substituting the bid,

\[
\text{edge}
=
V-(V+\epsilon-h).
\]

Simplifying,

\[
\text{edge}=h-\epsilon.
\]

An informed sell only occurs when

\[
\epsilon>h,
\]

so \(\epsilon\) is positive.

For positive \(\epsilon\),

\[
|\epsilon|=\epsilon.
\]

Therefore,

\[
h-\epsilon
=
h-|\epsilon|.
\]

Thus,

\[
\boxed{\text{edge}=h-|\epsilon|}
\]

for an informed sell as well.

Therefore, conditional on any informed execution,

\[
\boxed{\text{edge}=h-|\epsilon|}.
\]

Because an informed trade requires

\[
|\epsilon|>h,
\]

every informed execution has negative edge for the market maker.

---

## 4. Probability of an Informed Execution

To use the standard normal distribution, standardize \(\epsilon\).

Since

\[
\epsilon\sim N(0,\sigma^2),
\]

define

\[
Z=\frac{\epsilon}{\sigma}.
\]

Then

\[
Z\sim N(0,1).
\]

For the right tail,

\[
P(\epsilon>h)
=
P\left(\frac{\epsilon}{\sigma}>\frac{h}{\sigma}\right).
\]

Since

\[
Z=\frac{\epsilon}{\sigma},
\]

this becomes

\[
P(\epsilon>h)
=
P\left(Z>\frac{h}{\sigma}\right).
\]

Let

\[
\Phi(z)
\]

denote the standard normal cumulative distribution function. Thus, \(\Phi(z)\) is the probability that a standard normal random variable is less than or equal to \(z\).

Therefore,

\[
P\left(Z>\frac{h}{\sigma}\right)
=
1-\Phi\left(\frac{h}{\sigma}\right).
\]

So

\[
\boxed{
P(\epsilon>h)
=
1-\Phi\left(\frac{h}{\sigma}\right)
}.
\]

By symmetry of the normal distribution around zero,

\[
P(\epsilon<-h)
=
1-\Phi\left(\frac{h}{\sigma}\right).
\]

Therefore,

\[
P(|\epsilon|>h)
=
2\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right].
\]

Since an informed trader arrives with probability \(p_I\),

\[
\boxed{
P(\text{informed execution})
=
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]
}.
\]

---

## 5. Average Error Conditional on an Informed Trade

To calculate the average informed-trade edge, we need the average magnitude of the signal error after conditioning on the error being large enough to cause an informed execution.

Because the normal distribution is symmetric around zero, the magnitudes of left-tail observations

\[
\epsilon<-h
\]

have the same distribution as the positive values in the right tail

\[
\epsilon>h.
\]

After taking absolute values, the left and right tails therefore have the same magnitude distribution. Hence,

\[
\boxed{
E[|\epsilon|\mid|\epsilon|>h]
=
E[\epsilon\mid\epsilon>h]
}.
\]

The conditional expectation is

\[
E[\epsilon\mid\epsilon>h]
=
\frac{
\int_h^\infty \epsilon f(\epsilon)\,d\epsilon
}{
P(\epsilon>h)
}.
\]

Here,

\[
f(\epsilon)
=
\frac{1}{\sigma\sqrt{2\pi}}
e^{-\epsilon^2/(2\sigma^2)}
\]

is the normal probability density.

The density \(f(\epsilon)\) is not the probability of observing one exact value of \(\epsilon\). Instead, it describes how probability is distributed over intervals.

The numerator

\[
\int_h^\infty \epsilon f(\epsilon)\,d\epsilon
\]

is not itself a probability. It is the probability-weighted contribution of the values in the right tail.

Dividing by

\[
P(\epsilon>h)
\]

renormalizes the right tail so that the result is the average value only among observations satisfying

\[
\epsilon>h.
\]

Now consider the numerator:

\[
\int_h^\infty
\epsilon
\frac{1}{\sigma\sqrt{2\pi}}
e^{-\epsilon^2/(2\sigma^2)}
\,d\epsilon.
\]

First pull the constant outside the integral:

# \[

\frac{1}{\sigma\sqrt{2\pi}}
\int_h^\infty
\epsilon
e^{-\epsilon^2/(2\sigma^2)}
\,d\epsilon.
\]

Now,

\[
\frac{d}{d\epsilon}
\left(
-\frac{\epsilon^2}{2\sigma^2}
\right)
=
-\frac{\epsilon}{\sigma^2}.
\]

Therefore,

\[
\frac{d}{d\epsilon}
\left(
e^{-\epsilon^2/(2\sigma^2)}
\right)
=
-\frac{\epsilon}{\sigma^2}
e^{-\epsilon^2/(2\sigma^2)}.
\]

Rearranging,

\[
\epsilon
e^{-\epsilon^2/(2\sigma^2)}
=
-\sigma^2
\frac{d}{d\epsilon}
\left(
e^{-\epsilon^2/(2\sigma^2)}
\right).
\]

Thus,

\[
\int_h^\infty
\epsilon
e^{-\epsilon^2/(2\sigma^2)}
\,d\epsilon
=
-\sigma^2
\left[
e^{-\epsilon^2/(2\sigma^2)}
\right]\_h^\infty.
\]

As \(\epsilon\) approaches infinity,

\[
e^{-\epsilon^2/(2\sigma^2)}
\rightarrow 0.
\]

Therefore,

# \[

\sigma^2
e^{-h^2/(2\sigma^2)}.
\]

Now put back the constant that was pulled outside the integral:

\[
\int_h^\infty
\epsilon f(\epsilon)\,d\epsilon
=
\frac{1}{\sigma\sqrt{2\pi}}
\sigma^2
e^{-h^2/(2\sigma^2)}.
\]

Simplifying,

\[
\boxed{
\int_h^\infty
\epsilon f(\epsilon)\,d\epsilon
=
\frac{\sigma}{\sqrt{2\pi}}
e^{-h^2/(2\sigma^2)}
}.
\]

Define the standard normal density

\[
\phi(z)
=
\frac{1}{\sqrt{2\pi}}
e^{-z^2/2}.
\]

The notation is

\[
\phi(z)
=
\text{standard normal density},
\]

while

\[
\Phi(z)
=
\text{standard normal cumulative probability}.
\]

Since

\[
z=\frac{h}{\sigma},
\]

the numerator can be written as

\[
\sigma
\phi\left(\frac{h}{\sigma}\right).
\]

Also,

\[
P(\epsilon>h)
=
1-\Phi\left(\frac{h}{\sigma}\right).
\]

Therefore,

\[
\boxed{
E[\epsilon\mid\epsilon>h]
=
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}
}.
\]

By symmetry,

\[
\boxed{
E[|\epsilon|\mid|\epsilon|>h]
=
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}
}.
\]

---

## 6. Average Edge of an Informed Execution

We already found that

\[
\text{edge}=h-|\epsilon|
\]

for an informed execution.

Therefore,

\[
E[\text{edge}\mid\text{informed execution}]
=
h-
E[|\epsilon|\mid|\epsilon|>h].
\]

Substituting the conditional expectation,

\[
\boxed{
E[\text{edge}\mid\text{informed execution}]
=
h-
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}
}.
\]

---

## 7. Noise Trader Edge

A noise trader ignores the true value and chooses its trading direction randomly.

If the noise trader buys from the market maker,

\[
\text{edge}
=
\text{ask}-V
=
h+\epsilon.
\]

If the noise trader sells to the market maker,

\[
\text{edge}
=
V-\text{bid}
=
h-\epsilon.
\]

Since buy and sell directions are equally likely and

\[
E[\epsilon]=0,
\]

the average edge conditional on a noise trade is

\[
\boxed{
E[\text{edge}\mid\text{noise execution}]=h
}.
\]

A noise trader arrives with probability

\[
1-p_I
\]

and trades with probability \(q\).

Therefore,

\[
\boxed{
P(\text{noise execution})
=
(1-p_I)q
}.
\]

For the baseline parameters,

\[
(1-0.40)(0.30)
=
0.18.
\]

---

## 8. Overall Expected Execution Edge

There are two mutually exclusive ways an execution can occur in the baseline model:

- a noise trader executes, or
- an informed trader executes.

Let \(N\) denote a noise execution and \(I\) denote an informed execution.

Then the average execution edge conditional on an actual trade is

\[
E[\text{edge}\mid\text{execution}]
=
\frac{
P(N)E[\text{edge}\mid N]

- P(I)E[\text{edge}\mid I]
  }{
  P(N)+P(I)
  }.
  \]

The numerator is the expected edge contributed per random round by each possible type of execution.

The denominator is the probability that an execution occurs at all.

Dividing by the total execution probability gives the average edge conditional on an actual trade having occurred.

The probability of a noise execution is

\[
P(N)
=
(1-p_I)q.
\]

The probability of an informed execution is

\[
P(I)
=
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right].
\]

The expected edge conditional on a noise execution is

\[
E[\text{edge}\mid N]
=
h.
\]

The expected edge conditional on an informed execution is

\[
E[\text{edge}\mid I]
=
h-
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}.
\]

Substituting these quantities,

\[
E[\text{edge}\mid\text{execution}]
=
\frac{
(1-p_I)qh

- 2p_I[1-\Phi(h/\sigma)]
  \left[
  h-
  \frac{
  \sigma\phi(h/\sigma)
  }{
  1-\Phi(h/\sigma)
  }
  \right]
  }{
  (1-p_I)q
- 2p_I[1-\Phi(h/\sigma)]
  }.
  \]

The informed term simplifies because

\[
[1-\Phi(h/\sigma)]
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}
=
\sigma\phi(h/\sigma).
\]

Therefore the numerator becomes

\[
h\left(
(1-p_I)q

- 2p_I[1-\Phi(h/\sigma)]
  \right)

* 2p_I\sigma\phi(h/\sigma).
  \]

The expression in parentheses is exactly the denominator. Dividing through gives

\[
\boxed{
E[\text{edge}\mid\text{execution}]
=
h

- \frac{
  2p_I\sigma\phi(h/\sigma)
  }{
  (1-p_I)q

* 2p_I[1-\Phi(h/\sigma)]
  }
  }.
  \]

This is the mathematical prediction used by `analyze.py` for the fixed spread strategy.

---

## 9. Example: \(h=1\), \(\sigma=1\)

For the baseline parameters,

\[
h=1,
\qquad
\sigma=1,
\qquad
p_I=0.40,
\qquad
q=0.30.
\]

The informed trader trades when

\[
|\epsilon|>1.
\]

For one tail,

\[
P(\epsilon>1)
\approx 0.1587.
\]

Therefore the probability of an informed execution in a random round is

\[
2(0.40)(0.1587)
\approx 0.127.
\]

The probability of a noise execution is

\[
(0.60)(0.30)
=
0.18.
\]

The conditional average error in the right tail is approximately

\[
E[\epsilon\mid\epsilon>1]
\approx 1.525.
\]

Therefore the average informed execution edge is

\[
1-1.525
=
-0.525.
\]

The probability-weighted edge per random round is approximately

\[
0.18(1)

- # 0.127(-0.525)
  0.1133.
  \]

The probability that any execution occurs is approximately

\[
0.18+0.127
=
0.307.
\]

Therefore,

\[
E[\text{edge}\mid\text{execution}]
\approx
\frac{0.1133}{0.307}
\approx
0.369.
\]

---

## 10. Mathematical Prediction vs Simulation

The simulation was repeated 50 times for each signal-noise level using different fixed seeds.

For the fixed spread strategy, the mathematical predictions and simulated average execution edges were:

| Signal noise \(\sigma\) | Mathematical prediction | Simulation average | Difference |
| ----------------------: | ----------------------: | -----------------: | ---------: |
|                     0.5 |                  0.8910 |             0.8860 |    -0.0050 |
|                     1.0 |                  0.3693 |             0.3661 |    -0.0032 |
|                     2.0 |                 -0.3197 |            -0.3247 |    -0.0050 |
|                     4.0 |                 -1.4696 |            -1.4794 |    -0.0098 |

The simulation closely follows the mathematical prediction across all four tested signal-noise levels.

This provides a check that the fixed spread simulator is implementing the baseline model as intended.

It also shows the effect of increasing signal uncertainty: as \(\sigma\) increases, informed traders are able to exploit sufficiently mispriced quotes more often and the market maker's expected execution edge eventually becomes negative.

---

## 11. Scope of This Derivation

This derivation applies only to the fixed spread strategy in the original baseline model.

It does not yet derive the behavior of:

- the inventory-skew strategy,
- the uncertainty-aware strategy,
- a price-sensitive noise trader,
- an imperfectly informed trader, or
- more realistic order-book or strategic behavior.

Those model changes alter the trading probabilities or quote behavior and therefore require separate derivations.
