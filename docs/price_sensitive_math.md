# Price-Sensitive Uninformed Trader Mathematics

This document develops the first revision of the original uninformed-trader model.

The original model assumes that an uninformed trader trades with a fixed probability, regardless of the prices quoted by the market maker. This makes the model mathematically simple, but it also creates an important limitation: widening the spread does not reduce uninformed trading activity.

The revised model changes this assumption while keeping the rest of the market as controlled as possible.

The purpose of this model is to study how market-maker behavior changes when uninformed traders are still uncertain about the true value, but become sensitive to the prices they are offered.

---

## 1. Motivation for Revising the Original Model

In the original model, a noise trader arrives when the arriving trader is not informed.

The probability of an informed trader arriving is

\[
p_I = 0.40.
\]

Therefore, the probability of an uninformed trader arriving is

\[
1-p_I = 0.60.
\]

Conditional on an uninformed trader arriving, the original trader executes with the fixed probability

\[
q=0.30.
\]

Therefore,

\[
P(\text{uninformed execution})
=
(1-p_I)q.
\]

Using the baseline values,

\[
P(\text{uninformed execution})
=
(0.60)(0.30)
=
0.18.
\]

The important assumption is that

\[
q=0.30
\]

does not depend on the quoted prices.

Suppose the fixed-spread market maker uses

\[
\text{bid}=S-h
\]

and

\[
\text{ask}=S+h.
\]

If the market maker increases \(h\), both quotes move farther away from the signal.

However, under the original trader model,

\[
P(\text{trade}\mid\text{uninformed arrival})=0.30
\]

whether

\[
h=0.5,
\]

\[
h=1,
\]

or even

\[
h=3.
\]

This means that the market maker can charge a wider spread without losing any uninformed trading activity.

That creates a potentially important model artifact: widening the spread can appear increasingly attractive because uninformed traders continue trading at the same rate even as the prices become worse for them.

The new model is intended to remove this assumption.

---

## 2. What the New Trader Knows

The revised trader is called the

`PriceSensitiveUninformedTrader`.

The word "uninformed" is important.

The trader does not know the hidden true value

\[
V.
\]

Instead, the trader observes the same noisy signal as the market maker:

\[
S=V+\epsilon,
\]

where

\[
\epsilon\sim N(0,\sigma^2).
\]

This means that the trader and the market maker have approximately the same information about the asset value in this model.

This is a deliberate modeling choice.

The goal of this revision is to change the trader's response to prices without also giving the trader an informational advantage.

Therefore, the difference between the original uninformed trader and the price-sensitive uninformed trader is behavioral rather than informational.

The original trader ignores the quoted price.

The revised trader reacts to the quoted price.

Neither trader knows \(V\).

This also prepares the model for a later extension in which another trader may receive a better signal than the market maker. Keeping both sides on the same signal here allows price sensitivity and information quality to be studied separately.

---

## 3. Why the Trader Chooses a Side First

The trader does not inspect both the bid and ask and then strategically choose whichever trade looks most attractive.

Instead, it first randomly chooses whether it wants to buy or sell:

\[
P(\text{buy intention})=\frac12
\]

and

\[
P(\text{sell intention})=\frac12.
\]

After choosing a direction, it decides whether to execute based on the price offered on that side.

This is another deliberate modeling choice.

If the trader examined both sides and selected whichever action looked best, it would become a more strategic decision-maker.

The purpose of this model is narrower.

We want to preserve the random demand direction of the original noise trader while adding only one new behavior:

\[
\text{price-sensitive acceptance}.
\]

The sequence is therefore

\[
\boxed{
\text{random buy/sell intention}
\rightarrow
\text{observe relevant quote}
\rightarrow
\text{accept or reject}
}.
\]

Randomness still determines what the trader wants to do.

Price determines whether the trader actually does it.

---

## 4. Side-Specific Price Distance

Because the trader has already chosen a side, the relevant price depends on that side.

For a trader intending to buy, define

\[
d_{\text{buy}}
=
\text{ask}-S.
\]

This measures how far the ask is above the trader's reference signal.

A larger value means buying is more expensive relative to the trader's estimate.

For a trader intending to sell, define

\[
d_{\text{sell}}
=
S-\text{bid}.
\]

A larger value means the bid is farther below the trader's estimate and therefore less attractive for selling.

Using separate quantities for the two sides matters because some market-making strategies may shift the bid and ask asymmetrically relative to \(S\).

If the quotes are symmetric, the two distances are equal.

For the fixed-spread strategy,

\[
\text{ask}=S+h
\]

and

\[
\text{bid}=S-h.
\]

Therefore,

\[
d_{\text{buy}}
=
(S+h)-S
=
h
\]

and

\[
d_{\text{sell}}
=
S-(S-h)
=
h.
\]

Thus, under the fixed-spread strategy,

\[
\boxed{
d_{\text{buy}}=d_{\text{sell}}=h
}.
\]

---

## 5. Price-Sensitive Trade Probability

Let

\[
D(d)
\]

represent the probability that the uninformed trader accepts a quote that is distance \(d\) from its noisy reference signal.

We want this function to have several properties.

First,

\[
0\le D(d)\le1,
\]

because it represents a probability.

Second, larger distances should make execution less likely.

Therefore \(D(d)\) should decrease as \(d\) increases.

Third, the function should remain mathematically simple enough that its consequences can be derived directly.

A simple choice satisfying these goals is

\[
D(d)=e^{-\beta d},
\]

for positive \(d\), where

\[
\beta>0
\]

controls how strongly the trader reacts to price.

As \(d\) increases,

\[
e^{-\beta d}
\]

decreases.

Therefore increasingly unattractive prices lead to lower execution probability.

---

## 6. Why We Set \(D(1)=0.30\)

The value

\[
D(1)=0.30
\]

is not a mathematical necessity.

It is a deliberate calibration choice.

In the original fixed-spread experiment,

\[
h=1.
\]

For a fixed-spread strategy,

\[
d=h.
\]

Therefore the original experiment corresponds to

\[
d=1.
\]

The original noise trader trades with probability

\[
q=0.30
\]

conditional on an uninformed trader arriving.

The revised function \(D(d)\) is replacing this conditional trade probability.

To make the old and new trader models agree at the original reference spread, we require

\[
\boxed{
D(1)=0.30
}.
\]

This means:

> when the quote is exactly the same distance from the signal as in the original baseline model, the new trader should have the same execution probability as the old trader.

This gives us a useful control point.

At

\[
h=1,
\]

the two models behave the same in terms of uninformed execution probability.

Differences at other spreads can therefore be attributed to the newly introduced price sensitivity rather than simply changing the baseline trading rate.

---

## 7. Solving for \(\beta\)

We chose

\[
D(d)=e^{-\beta d}.
\]

The calibration condition is

\[
D(1)=0.30.
\]

Substituting \(d=1\),

\[
0.30=e^{-\beta}.
\]

Taking the natural logarithm of both sides,

\[
\ln(0.30)
=
\ln(e^{-\beta}).
\]

Since

\[
\ln(e^x)=x,
\]

we obtain

\[
\ln(0.30)=-\beta.
\]

Therefore,

\[
\boxed{
\beta=-\ln(0.30)
}.
\]

Numerically,

\[
\boxed{
\beta\approx1.204
}.
\]

The resulting trade-probability function is approximately

\[
\boxed{
D(d)=e^{-1.204d}
}.
\]

---

## 8. Keeping the Probability Between 0 and 1

If

\[
d<0,
\]

then

\[
e^{-\beta d}>1.
\]

But a probability cannot exceed 1.

A negative distance means that the quote is actually more favorable than the trader's reference signal.

For example, for a buyer,

\[
d_{\text{buy}}<0
\]

means

\[
\text{ask}<S.
\]

The model interprets such a quote as sufficiently attractive that the trader accepts it with probability 1.

Therefore the full trade-probability rule is

\[
\boxed{
D(d)=\min(1,e^{-\beta d})
}.
\]

For the fixed-spread experiment used here,

\[
h>0,
\]

so

\[
d=h>0.
\]

Therefore the cap does not affect the fixed-spread calculations in this document.

For these experiments,

\[
D(h)=e^{-\beta h}.
\]

---

## 9. Uninformed Execution Probability

Conditional on an uninformed trader arriving, the execution probability is

\[
D(h).
\]

An uninformed trader arrives with probability

\[
1-p_I.
\]

Therefore the probability of an uninformed execution in a random round is

\[
\boxed{
P(\text{uninformed execution})
=
(1-p_I)D(h)
}.
\]

For the price-sensitive model,

\[
\boxed{
P(\text{uninformed execution})
=
(1-p_I)e^{-\beta h}
}.
\]

This replaces the original expression

\[
(1-p_I)q.
\]

---

## 10. Expected Edge of an Uninformed Trade

For a fixed-spread strategy,

\[
\text{ask}=S+h
\]

and

\[
\text{bid}=S-h.
\]

Since

\[
S=V+\epsilon,
\]

the ask is

\[
\text{ask}=V+\epsilon+h
\]

and the bid is

\[
\text{bid}=V+\epsilon-h.
\]

If the uninformed trader buys from the market maker,

\[
\text{edge}
=
\text{ask}-V
=
h+\epsilon.
\]

If the uninformed trader sells to the market maker,

\[
\text{edge}
=
V-\text{bid}
=
h-\epsilon.
\]

The trader chooses buy and sell intentions with equal probability.

Also,

\[
E[\epsilon]=0.
\]

Therefore,

\[
E[\text{edge}\mid\text{uninformed execution}]
=
h.
\]

Thus,

\[
\boxed{
E[\text{edge}\mid\text{uninformed execution}]
=
h
}.
\]

The new model changes how frequently these trades occur, not the expected edge conditional on one of these symmetric uninformed trades occurring.

---

## 11. Informed Trader Behavior Is Unchanged

The informed trader from the original model still knows the exact true value

\[
V.
\]

Its behavior is unchanged.

As derived in the baseline mathematics, an informed trade occurs when

\[
|\epsilon|>h.
\]

Therefore,

\[
P(\text{informed execution})
=
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right].
\]

The expected edge conditional on an informed execution is

\[
E[\text{edge}\mid\text{informed execution}]
=
h-
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}.
\]

The only change in the revised model is the uninformed trader's execution probability.

---

## 12. Why Edge Per Round Matters

One possible measure is

\[
E[\text{edge}\mid\text{execution}],
\]

which asks how favorable an average executed trade is.

However, this measure does not account for how frequently trading occurs.

Suppose a very wide spread produces a large positive edge whenever somebody trades, but almost nobody is willing to trade.

The average edge per executed trade might still appear very good even though the market maker rarely receives any trades.

For the spread experiment, it is therefore useful to study

\[
\boxed{
E[\text{edge per round}]
}.
\]

This measures expected execution edge across every market opportunity, including rounds where no trade occurs.

It captures both:

\[
\text{quality of executions}
\]

and

\[
\text{frequency of executions}.
\]

---

## 13. Expected Edge Per Round

There are two possible sources of execution:

1. an uninformed trader executes;
2. an informed trader executes.

Therefore,

\[
E[\text{edge per round}]
=
P(U)E[\text{edge}\mid U]
+
P(I)E[\text{edge}\mid I],
\]

where \(U\) denotes an uninformed execution and \(I\) denotes an informed execution.

For the price-sensitive uninformed trader,

\[
P(U)
=
(1-p_I)D(h).
\]

Also,

\[
E[\text{edge}\mid U]
=
h.
\]

Therefore the uninformed contribution is

\[
\boxed{
(1-p_I)D(h)h
}.
\]

The informed execution probability is

\[
P(I)
=
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right].
\]

The informed conditional edge is

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

Therefore the informed contribution is

\[
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]
\left[
h-
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}
\right].
\]

Combining the two contributions,

\[
E[\text{edge per round}]
=
(1-p_I)D(h)h
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]
\left[
h-
\frac{
\sigma\phi(h/\sigma)
}{
1-\Phi(h/\sigma)
}
\right].
\]

The informed term can be simplified.

Multiplying through gives

\[
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]h
-
2p_I\sigma\phi\left(\frac{h}{\sigma}\right).
\]

Therefore,

\[
\boxed{
E[\text{edge per round}]
=
(1-p_I)D(h)h
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]h
-
2p_I\sigma\phi\left(\frac{h}{\sigma}\right)
}.
\]

Since

\[
D(h)=e^{-\beta h},
\]

the price-sensitive model becomes

\[
\boxed{
E[\text{edge per round}]
=
(1-p_I)e^{-\beta h}h
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]h
-
2p_I\sigma\phi\left(\frac{h}{\sigma}\right)
}.
\]

---

## 14. Comparison With the Original Model

For the original trader,

\[
D(h)=q=0.30
\]

for every value of \(h\).

Therefore,

\[
E[\text{edge per round}]
=
(1-p_I)qh
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]h
-
2p_I\sigma\phi\left(\frac{h}{\sigma}\right).
\]

The only difference between the two formulas is the uninformed-trade probability:

\[
q
\]

in the original model, compared with

\[
e^{-\beta h}
\]

in the price-sensitive model.

The informed-trader contribution is unchanged.

---

## 15. Spread Experiment

For this experiment, signal noise is held fixed at

\[
\sigma=1.
\]

The probability of an informed trader arriving remains

\[
p_I=0.40.
\]

The price-sensitivity parameter is

\[
\beta=-\ln(0.30).
\]

The tested half-spreads are

\[
h\in
\{
0.5,\,
1.0,\,
1.5,\,
2.0,\,
3.0
\}.
\]

Each configuration is simulated 50 times using different fixed seeds.

Only two things are compared:

- the original uninformed trader;
- the price-sensitive uninformed trader.

The market maker uses the fixed-spread strategy in both cases.

This isolates the effect of changing the uninformed trader's response to price.

---

## 16. Predicted Uninformed Trade Rates

The original trader has

\[
P(\text{trade}\mid\text{uninformed arrival})
=
0.30
\]

at every spread.

For the revised trader,

\[
P(\text{trade}\mid\text{uninformed arrival})
=
e^{-\beta h}.
\]

Using

\[
\beta=-\ln(0.30),
\]

the predictions are:

| Half-spread \(h\) | Original model | Price-sensitive model |
| ----------------: | -------------: | --------------------: |
|               0.5 |         0.3000 |                0.5477 |
|               1.0 |         0.3000 |                0.3000 |
|               1.5 |         0.3000 |                0.1643 |
|               2.0 |         0.3000 |                0.0900 |
|               3.0 |         0.3000 |                0.0270 |

The two models meet at

\[
h=1,
\]

because of the calibration condition

\[
D(1)=0.30.
\]

---

## 17. Predicted vs Simulated Uninformed Trade Rate

The repeated simulations produced:

| \(h\) | Model           | Predicted | Simulation |
| ----: | --------------- | --------: | ---------: |
|   0.5 | Original        |    0.3000 |     0.3030 |
|   0.5 | Price-sensitive |    0.5477 |     0.5444 |
|   1.0 | Original        |    0.3000 |     0.3030 |
|   1.0 | Price-sensitive |    0.3000 |     0.2992 |
|   1.5 | Original        |    0.3000 |     0.3030 |
|   1.5 | Price-sensitive |    0.1643 |     0.1628 |
|   2.0 | Original        |    0.3000 |     0.3030 |
|   2.0 | Price-sensitive |    0.0900 |     0.0891 |
|   3.0 | Original        |    0.3000 |     0.3030 |
|   3.0 | Price-sensitive |    0.0270 |     0.0272 |

The simulated execution rates closely follow the mathematical probabilities.

---

## 18. Predicted vs Simulated Edge Per Round

For the original model:

| \(h\) | Mathematical prediction | Simulation |
| ----: | ----------------------: | ---------: |
|   0.5 |                -0.06824 |   -0.06966 |
|   1.0 |                 0.11335 |    0.11287 |
|   1.5 |                 0.24655 |    0.24668 |
|   2.0 |                 0.35321 |    0.35404 |
|   3.0 |                 0.53969 |    0.54165 |

For the price-sensitive model:

| \(h\) | Mathematical prediction | Simulation |
| ----: | ----------------------: | ---------: |
|   0.5 |                 0.00608 |    0.00465 |
|   1.0 |                 0.11335 |    0.11348 |
|   1.5 |                 0.12444 |    0.12345 |
|   2.0 |                 0.10121 |    0.09989 |
|   3.0 |                 0.04829 |    0.04811 |

The simulations again closely follow the derived mathematical predictions.

---

## 19. Main Result of the Model Revision

The original model shows steadily increasing expected edge per round across the tested spreads:

\[
-0.068
\rightarrow
0.113
\rightarrow
0.247
\rightarrow
0.353
\rightarrow
0.540.
\]

This occurs because uninformed traders continue executing with approximately the same probability even as the spread becomes wider.

The revised model behaves differently:

\[
0.006
\rightarrow
0.113
\rightarrow
0.124
\rightarrow
0.101
\rightarrow
0.048.
\]

Initially, increasing the spread improves the market maker's expected edge.

However, after some point, the reduction in uninformed trading activity outweighs the benefit of charging a wider spread.

Therefore the revised model produces a tradeoff between

\[
\text{larger edge when a trade occurs}
\]

and

\[
\text{lower probability of receiving that trade}.
\]

Among the tested values,

\[
h=1.5
\]

produces the highest simulated edge per round for the price-sensitive model.

This does not mean that \(h=1.5\) is a universally optimal market-making spread.

It is only the best spread among the tested values under the assumptions and parameter values of this toy model.

---

## 20. Interpretation

The main result is not that one particular spread is best.

The more important result is that a modeling assumption about counterparty behavior changes the qualitative conclusion of the experiment.

Under the original assumption,

\[
P(\text{uninformed trade})
\]

does not respond to price.

This makes wider spreads continue to look increasingly attractive.

After introducing price-sensitive uninformed trading,

\[
P(\text{uninformed trade})
\]

falls as the quote becomes less attractive.

The market maker now faces a genuine tradeoff.

This demonstrates why apparently small assumptions about how counterparties behave can strongly affect conclusions drawn from a market simulation.

---

## 21. Scope of This Model

The price-sensitive uninformed trader is still intentionally simplified.

It:

- does not know the true value \(V\);
- sees the same noisy signal \(S\) as the market maker;
- randomly chooses whether it wants to buy or sell;
- reacts only to the quote on its chosen side;
- does not compare both sides strategically;
- does not learn from previous rounds;
- does not adapt to the market maker over time.

These limitations are deliberate.

This model changes one feature of the original uninformed trader while keeping the rest of the environment controlled.

A later model can change the information structure separately by introducing a trader that receives a better signal than the market maker without directly observing the true value.

---

## 22. Finding the Best Spread Mathematically

The spread experiment tested only the discrete values

\[
h\in\{0.5,1.0,1.5,2.0,3.0\}.
\]

Among those values, the price-sensitive model produced its largest simulated edge per round at

\[
h=1.5.
\]

However, this does not tell us whether \(1.5\) is actually the best spread predicted by the mathematical model.

The expected edge per round is a continuous function of \(h\), so we can instead ask:

> For a given level of signal noise \(\sigma\), what value of \(h\) maximizes expected edge per round?

From the previous derivation,

\[
F(h)
=
E[\text{edge per round}]
\]

is

\[
F(h)
=
(1-p_I)e^{-\beta h}h
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]h
-
2p_I\sigma
\phi\left(\frac{h}{\sigma}\right).
\]

To find a maximum, we differentiate \(F(h)\) with respect to \(h\).

A candidate optimum occurs where

\[
F'(h)=0.
\]

---

## 23. Differentiating the Uninformed-Trader Term

The first term is

\[
(1-p_I)he^{-\beta h}.
\]

The constant

\[
1-p_I
\]

does not depend on \(h\), so we only need to differentiate

\[
he^{-\beta h}.
\]

This is a product of

\[
h
\]

and

\[
e^{-\beta h},
\]

so we use the product rule:

\[
\frac{d}{dh}[uv]
=
u'v+uv'.
\]

Let

\[
u=h
\]

and

\[
v=e^{-\beta h}.
\]

Then

\[
u'=1.
\]

For the second part,

\[
\frac{d}{dh}
e^{-\beta h}
=
-\beta e^{-\beta h}.
\]

Therefore,

\[
\frac{d}{dh}
\left[
he^{-\beta h}
\right]
=
e^{-\beta h}
-
\beta h e^{-\beta h}.
\]

Factoring out the common exponential term gives

\[
e^{-\beta h}(1-\beta h).
\]

Putting the constant \(1-p_I\) back,

\[
\boxed{
\frac{d}{dh}
\left[
(1-p_I)he^{-\beta h}
\right]
=
(1-p_I)e^{-\beta h}(1-\beta h)
}.
\]

---

## 24. Differentiating the First Informed-Trader Term

The next term is

\[
2p_Ih
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right].
\]

Again, \(2p_I\) is constant with respect to \(h\).

We therefore differentiate

\[
h
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right].
\]

This is also a product.

Let

\[
u=h
\]

and

\[
v=
1-\Phi\left(\frac{h}{\sigma}\right).
\]

Then

\[
u'=1.
\]

To find \(v'\), recall that the derivative of the standard normal cumulative distribution function is the standard normal density:

\[
\Phi'(z)=\phi(z).
\]

Here the input to \(\Phi\) is

\[
\frac{h}{\sigma}.
\]

Therefore, by the chain rule,

\[
\frac{d}{dh}
\Phi\left(\frac{h}{\sigma}\right)
=
\phi\left(\frac{h}{\sigma}\right)
\frac{1}{\sigma}.
\]

Since our expression contains

\[
1-\Phi\left(\frac{h}{\sigma}\right),
\]

its derivative is

\[
-\frac{1}{\sigma}
\phi\left(\frac{h}{\sigma}\right).
\]

Therefore,

\[
v'
=
-\frac{1}{\sigma}
\phi\left(\frac{h}{\sigma}\right).
\]

Using the product rule,

\[
\frac{d}{dh}
\left[
h
\left(
1-\Phi\left(\frac{h}{\sigma}\right)
\right)
\right]
\]

becomes

\[
1-\Phi\left(\frac{h}{\sigma}\right)
-
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right).
\]

Putting \(2p_I\) back gives

\[
\boxed{
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
-
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
\right]
}.
\]

---

## 25. Differentiating the Second Informed-Trader Term

The final term is

\[
-2p_I\sigma
\phi\left(\frac{h}{\sigma}\right).
\]

The constant outside the density is

\[
-2p_I\sigma.
\]

The standard normal density is

\[
\phi(z)
=
\frac{1}{\sqrt{2\pi}}e^{-z^2/2}.
\]

Its derivative is

\[
\boxed{
\phi'(z)=-z\phi(z)
}.
\]

In this case,

\[
z=\frac{h}{\sigma}.
\]

Therefore,

\[
\phi'\left(\frac{h}{\sigma}\right)
=
-\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right).
\]

However, we are differentiating with respect to \(h\), not with respect to \(z\).

Since

\[
\frac{d}{dh}
\left(
\frac{h}{\sigma}
\right)
=
\frac{1}{\sigma},
\]

the chain rule gives

\[
\frac{d}{dh}
\phi\left(\frac{h}{\sigma}\right)
=
-\frac{h}{\sigma^2}
\phi\left(\frac{h}{\sigma}\right).
\]

Now multiply by the outside constant:

\[
-2p_I\sigma
\left[
-\frac{h}{\sigma^2}
\phi\left(\frac{h}{\sigma}\right)
\right].
\]

The two negative signs cancel:

\[
2p_I\sigma
\frac{h}{\sigma^2}
\phi\left(\frac{h}{\sigma}\right).
\]

One factor of \(\sigma\) cancels, giving

\[
\boxed{
2p_I
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
}.
\]

---

## 26. Cancellation in the Derivative

Combining all three derivative terms gives

\[
F'(h)
=
(1-p_I)e^{-\beta h}(1-\beta h)
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
-
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
\right]
+
2p_I
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right).
\]

Notice that the terms

\[
-2p_I
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
\]

and

\[
+2p_I
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
\]

are exact opposites.

Therefore they cancel:

\[
-2p_I
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
+
2p_I
\frac{h}{\sigma}
\phi\left(\frac{h}{\sigma}\right)
=
0.
\]

This leaves the much simpler derivative

\[
\boxed{
F'(h)
=
(1-p_I)e^{-\beta h}(1-\beta h)
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]
}.
\]

This is the derivative used to locate the best spread predicted by the model.

---

## 27. Solving for the Optimal Spread

The optimal spread is found by solving

\[
\boxed{
F'(h)=0
}.
\]

Therefore,

\[
(1-p_I)e^{-\beta h}(1-\beta h)
+
2p_I
\left[
1-\Phi\left(\frac{h}{\sigma}\right)
\right]
=
0.
\]

Because this equation contains both an exponential function and the normal cumulative distribution function, there is not a simple algebraic rearrangement that isolates \(h\).

For that reason, the project solves this equation numerically.

The program uses a simple bisection search.

The idea is to start with an interval containing the solution.

The midpoint of the interval is tested.

If

\[
F'(h)>0,
\]

then \(F(h)\) is still increasing at that point, so the search moves to larger values of \(h\).

If

\[
F'(h)<0,
\]

then \(F(h)\) is decreasing at that point, so the search moves to smaller values of \(h\).

Repeating this process continually cuts the search interval in half until the location where

\[
F'(h)=0
\]

is approximated very closely.

The implementation searches within

\[
0\le h\le50,
\]

which contains the solutions for all signal-noise levels examined here.

---

## 28. Optimal Spread When \(\sigma=1\)

For the spread experiment,

\[
p_I=0.40,
\]

\[
\beta=-\ln(0.30),
\]

and

\[
\sigma=1.
\]

Solving

\[
F'(h)=0
\]

numerically gives

\[
\boxed{
h^*\approx1.3335
}.
\]

At this spread,

\[
\boxed{
F(h^*)\approx0.12675
}.
\]

This result is consistent with the earlier discrete experiment.

The tested spreads nearest to the mathematical optimum were

\[
h=1
\]

and

\[
h=1.5.
\]

Their predicted edge per round values were approximately

\[
F(1)=0.11335
\]

and

\[
F(1.5)=0.12444.
\]

The continuous optimum lies between them at approximately

\[
h=1.3335.
\]

Therefore the earlier result that \(h=1.5\) was the best tested spread did not mean that \(1.5\) was the exact mathematical optimum.

It was simply the best point in the discrete set that was tested.

---

## 29. How Signal Uncertainty Changes the Optimal Spread

The same calculation can be repeated while changing the signal-noise level \(\sigma\).

The parameters

\[
p_I=0.40
\]

and

\[
\beta=-\ln(0.30)
\]

are held constant.

The resulting optimal spreads are:

| Signal noise \(\sigma\) | Optimal half-spread \(h^*\) | Maximum predicted edge per round |
|---:|---:|---:|
| 0.5 | 0.9354 | 0.17722 |
| 1.0 | 1.3335 | 0.12675 |
| 2.0 | 3.9362 | 0.00587 |
| 4.0 | 32.2039 | approximately 0 |

The general pattern is

\[
\boxed{
\text{higher signal uncertainty}
\rightarrow
\text{wider optimal spread}
}.
\]

When the market maker's signal is relatively accurate, such as

\[
\sigma=0.5,
\]

it can quote relatively close to its signal while still maintaining positive expected edge.

As the signal becomes less reliable, informed traders have more opportunities to exploit quotes based on large signal errors.

The market maker therefore needs a wider spread to protect itself.

---

## 30. Wider Spreads Do Not Fully Solve the Information Problem

The increase in optimal spread does not mean that increased uncertainty can simply be solved by widening the quotes.

The maximum expected edge also decreases as \(\sigma\) increases.

At

\[
\sigma=0.5,
\]

the maximum predicted edge per round is approximately

\[
0.17722.
\]

At

\[
\sigma=1,
\]

it falls to approximately

\[
0.12675.
\]

At

\[
\sigma=2,
\]

it falls dramatically to approximately

\[
0.00587.
\]

Therefore, although widening the spread protects the market maker from some informed trading, it does not remove the underlying disadvantage created by having a poor estimate of the true value.

The market maker protects itself partly by making trades less likely.

This creates another tradeoff:

\[
\text{protection from adverse selection}
\]

versus

\[
\text{continued participation in the market}.
\]

---

## 31. Interpreting the \(\sigma=4\) Result

When

\[
\sigma=4,
\]

the numerical solution gives

\[
h^*\approx32.2039.
\]

The predicted edge per round at this point is approximately

\[
1.22\times10^{-16},
\]

which is effectively zero.

This result should not be interpreted as saying that a half-spread of approximately \(32\) is a practically attractive market-making strategy.

Instead, the result reflects the structure of this toy model.

The market maker's signal has become so noisy that the model finds very little opportunity for positive expected edge.

The extremely wide spread makes uninformed executions almost disappear and also makes informed executions extremely rare.

In this sense, the mathematical optimum is approaching the behavior of not trading.

The market maker is protecting itself from its information disadvantage by posting quotes that almost nobody accepts.

Therefore the more useful interpretation is

\[
\boxed{
\text{when uncertainty becomes sufficiently severe,}
}
\]

\[
\boxed{
\text{the model favors withdrawing from trading rather than}
}
\]

\[
\boxed{
\text{continuing to quote actively at unfavorable prices.}
}
\]

---

## 32. What This Adds to the Model

The original spread experiment showed that making uninformed traders price-sensitive changes the qualitative effect of widening the spread.

The optimization analysis adds another result.

The best spread is not fixed.

It depends on the market maker's level of uncertainty.

As uncertainty increases,

\[
h^*
\]

also increases.

At the same time, the maximum achievable expected edge falls.

Therefore the model suggests two related effects of worsening information:

\[
\text{greater uncertainty}
\rightarrow
\text{more defensive quoting},
\]

and

\[
\text{greater uncertainty}
\rightarrow
\text{lower useful trading opportunity}.
\]

This result also motivates the next extension of the project.

So far, one trader knows the exact true value \(V\), while the market maker and the price-sensitive uninformed trader use the noisy signal \(S\).

A later model can weaken this extreme information difference by introducing a `BetterSignalTrader`: a trader that still does not know \(V\), but receives a more accurate noisy signal than the market maker.
