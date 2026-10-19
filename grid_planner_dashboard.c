#include <stdio.h>
#include <math.h>

#define MAX 20          // maximum number of buildings

// a building has a name and a position (in meters)
struct Building {
    char name[30];
    float x, y;
};

// a possible cable between building u and building v
struct Cable {
    int u, v;
    float cost;
};

struct Building b[MAX];
struct Cable cables[200];
int chosen[200];        // NEW: chosen[i] = 1 if cable i was taken by Kruskal's
int parent[MAX];

// find which group building x belongs to
int find(int x) {
    if (parent[x] == x)
        return x;
    return parent[x] = find(parent[x]);
}

// straight-line distance between building i and building j
float distanceBetween(int i, int j) {
    float dx = b[i].x - b[j].x;
    float dy = b[i].y - b[j].y;
    return sqrt(dx * dx + dy * dy);
}

int main() {
    int n, e = 0;
    float rate, total = 0, naive = 0;

    // ---------- 1. INPUT ----------
    printf("=== PowerPath Planner ===\n");
    printf("Number of buildings (building 0 = substation / power source): ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Building %d (name x y): ", i);
        scanf("%s %f %f", b[i].name, &b[i].x, &b[i].y);
    }

    printf("Cable cost per meter (Rs): ");
    scanf("%f", &rate);

    // ---------- 2. MAKE ALL POSSIBLE CABLES ----------
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            cables[e].u = i;
            cables[e].v = j;
            cables[e].cost = distanceBetween(i, j) * rate;
            e++;
        }
    }

    // ---------- 3. SORT CABLES (cheapest first) ----------
    for (int i = 0; i < e - 1; i++) {
        for (int j = 0; j < e - i - 1; j++) {
            if (cables[j].cost > cables[j + 1].cost) {
                struct Cable temp = cables[j];
                cables[j] = cables[j + 1];
                cables[j + 1] = temp;
            }
        }
    }

    // ---------- 4. KRUSKAL'S ALGORITHM ----------
    for (int i = 0; i < n; i++)
        parent[i] = i;                   // every building is its own group

    printf("\n--- Cables to build (minimum cost) ---\n");
    for (int i = 0; i < e; i++) {
        int a = find(cables[i].u);
        int c = find(cables[i].v);

        if (a != c) {                    // not connected yet, so take it
            parent[a] = c;               // join the two groups
            chosen[i] = 1;               // NEW: remember that this cable was taken
            total += cables[i].cost;
            printf("%s <-> %s : Rs %.0f\n",
                   b[cables[i].u].name, b[cables[i].v].name, cables[i].cost);
        }
    }
    printf("Total minimum cost = Rs %.0f\n", total);

    // ---------- 5. COMPARE WITH THE NAIVE WAY ----------
    for (int i = 1; i < n; i++)
        naive += distanceBetween(0, i) * rate;

    printf("\nIf every building is wired directly to the substation: Rs %.0f\n", naive);
    printf("Money saved = Rs %.0f (%.1f%%)\n",
           naive - total, (naive - total) / naive * 100);

    // ---------- 6. NEW: SAVE THE RESULT FOR THE DASHBOARD ----------
    FILE *fp = fopen("plan.js", "w");
    if (fp != NULL) {
        fprintf(fp, "const plan = {\n");
        fprintf(fp, "  total: %.0f, naive: %.0f,\n", total, naive);
        fprintf(fp, "  buildings: [\n");
        for (int i = 0; i < n; i++)
            fprintf(fp, "    {name: \"%s\", x: %.1f, y: %.1f},\n", b[i].name, b[i].x, b[i].y);
        fprintf(fp, "  ],\n");
        fprintf(fp, "  cables: [\n");
        for (int i = 0; i < e; i++)
            fprintf(fp, "    {u: %d, v: %d, cost: %.2f, chosen: %d},\n",
                    cables[i].u, cables[i].v, cables[i].cost, chosen[i]);
        fprintf(fp, "  ]\n};\n");
        fclose(fp);
        printf("\nDashboard data saved to plan.js. Now open dashboard.html in your browser.\n");
    }

    return 0;
}
