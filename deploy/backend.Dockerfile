FROM python:3.14-trixie AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/
WORKDIR /src
COPY engine ./engine
COPY bindings ./bindings
COPY backend ./backend

WORKDIR /src/backend
RUN uv sync --frozen --no-dev --no-editable

FROM python:3.14-slim-trixie

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/
WORKDIR /src/backend
COPY --from=build /src/backend /src/backend
EXPOSE 5000
CMD ["uv", "run", "backend"]
