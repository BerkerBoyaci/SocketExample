# ── Build stage ──────────────────────────────────────────────────────────────
FROM gcc:13-bookworm AS builder

WORKDIR /app
COPY src/    ./src/
COPY Makefile .
RUN make all

# ── Server image ──────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS server

WORKDIR /app
COPY --from=builder /app/build/server .
EXPOSE 8080
CMD ["./server"]

# ── Client image ──────────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS client

WORKDIR /app
COPY --from=builder /app/build/client .
CMD ["./client", "server"]
